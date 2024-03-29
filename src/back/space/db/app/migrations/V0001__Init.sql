/* pgmigrate-encoding: utf-8 */
CREATE SCHEMA IF NOT EXISTS space;

CREATE EXTENSION IF NOT EXISTS "uuid-ossp";

CREATE TABLE space.space (
	id UUID PRIMARY KEY DEFAULT UUID_GENERATE_V4(),
	name TEXT NOT NULL,
	key VARCHAR(36) NOT NULL,
	requestsAllowed BOOLEAN NOT NULL,
	createdAt TIMESTAMP NOT NULL DEFAULT NOW()
);

CREATE UNIQUE INDEX idx_space_key ON space.space (key);

CREATE TABLE space.invitation (
	id SERIAL PRIMARY KEY,
	spaceId UUID NOT NULL REFERENCES space.space (id),
	creatorId VARCHAR(40) NOT NULL,
	userId VARCHAR(40) NOT NULL,
	role SMALLINT,
	createdAt TIMESTAMP NOT NULL DEFAULT NOW()
);

CREATE UNIQUE INDEX idx_space_invitation ON space.invitation (spaceId, userId);

CREATE FUNCTION check_user() RETURNS trigger AS $$
BEGIN
	PERFORM * FROM space.user WHERE userId = NEW.userId AND spaceId = NEW.spaceId;
	IF FOUND THEN
		RETURN NULL;
	END IF;
	RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER check_user_presents
	BEFORE INSERT ON space.invitation
	FOR EACH ROW EXECUTE PROCEDURE check_user();

CREATE TABLE space.link (
	id UUID PRIMARY KEY DEFAULT UUID_GENERATE_V4(),
	spaceId UUID NOT NULL REFERENCES space.space (id),
	creatorId VARCHAR(40) NOT NULL,
	name TEXT NOT NULL,
	createdAt TIMESTAMP NOT NULL DEFAULT NOW(),
	expiredAt TIMESTAMP NOT NULL
);

CREATE TABLE space.user (
	spaceId UUID NOT NULL REFERENCES space.space (id),
	userId VARCHAR(40) NOT NULL,
	isOwner BOOLEAN NOT NULL,
	joinedAt TIMESTAMP NOT NULL DEFAULT NOW(),
	role SMALLINT NOT NULL,

	PRIMARY KEY (spaceId, userId)
);

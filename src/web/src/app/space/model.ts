export interface SpaceServiceInfo {
	canCreate: boolean;
	invitationSize: number;
}

export interface SpaceListResponse {
	list: Space[];
}

export interface Space {
	id: string;
	name: string;
	key: string;
	requestsAllowed: boolean;
	createdAt: Date;
}

export interface SpaceInvitation {
	id: number,
	spaceId: string;
	creatorId: string;
	userId: string;
	role: string;
	createdAt: Date;
}

export interface SpaceLink {
	id: string;
	spaceId: string;
	creatorId: string;
	name: string;
	createdAt: Date;
	expiredAt: Date;
}

export interface SpaceUser {
	spaceId: string;
	userId: string;
	isOwner: boolean;
	joinedAt: Date;
	role: string;
}

export type SpaceFields = {
	spaceName: string;
}

export enum SpaceRole {
	Unknown = "",
	Guest = "guest",
	User = "user",
	Admin = "admin",
}
import { Component, OnInit, ViewChild, Inject} from '@angular/core';
import { ActivatedRoute } from '@angular/router';
import { MatPaginator } from '@angular/material/paginator';
import { DOCUMENT } from '@angular/common';
import { FormGroup, FormBuilder, Validators} from '@angular/forms';
import { startWith, map, debounceTime, distinctUntilChanged, switchMap } from 'rxjs/operators';
import { Observable } from 'rxjs';
import { MatOption } from '@angular/material/core';

import { Space, SpaceLink, SpaceUser, SpaceRole } from '../model';

import { SpaceService } from '../service';
import { UserService } from '../../user/service';
import { User, UserFields } from '../../user/model';

type SpaceUserDetail = SpaceUser & UserFields;

@Component({
	selector: 'app-space-detail',
	templateUrl: './component.html',
	styleUrls: ['./component.css', '../common.css']
})
export class SpaceDetailComponent implements OnInit {
	SpaceRole = SpaceRole;

	currentSpace: Space = {} as Space;
	currentUserId: string;

	users: SpaceUserDetail[] = [];

	pageSize = {
		invitations: 7,
		links: 7,
		users: 10
	};

	usersTotal: number;

	@ViewChild('usersPaginator') usersPaginator: MatPaginator;

	constructor(
		private route: ActivatedRoute,
		private space: SpaceService,
		private user: UserService,
	) {
	}

	ngOnInit() {
		this.currentUserId = this.user.info.id;

		const pageSizeStr = localStorage.getItem('spaceDetailPageSize');
		const pageSize = JSON.parse(pageSizeStr);
		if (pageSize?.invitations && pageSize?.links && pageSize?.users) {
			this.pageSize = pageSize;
		}

		const key = this.route.snapshot.paramMap.get('key');
		this.getInitData(key);
	}

	getInitData(key: string) {
		this.space.getByKey(key)
			.subscribe(res => {
				this.currentSpace = res;
				this.getUsers(this.pageSize.users, 0);
			});
	}

	getUsers(limit: number, page: number) {
		this.savePageSize("users", limit);
		this.space.getUserList(this.currentSpace.id, limit, page)
			.subscribe(res => {
				this.users = res.list as SpaceUserDetail[];
				this.usersTotal = res.total;
				this.user.fillFields(this.users);
			});
	}

	onUserDelBtn(user: SpaceUser) {
		this.space.delUserById(user.userId, this.currentSpace.id)
			.subscribe(_ => {
				if (this.usersPaginator.pageIndex == 0) {
					this.getUsers(this.pageSize.users, 0);
				} else {
					this.usersPaginator.firstPage();
				}
			});
	}

	savePageSize(id: string, limit: number) {
		if (this.pageSize[id] == limit)
			return;

		this.pageSize[id] = limit;
		localStorage.setItem('spaceDetailPageSize', JSON.stringify(this.pageSize));
	}
}

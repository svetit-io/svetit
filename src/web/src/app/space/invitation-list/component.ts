import { Component, OnInit, ViewChild, Input, Output, EventEmitter, inject } from '@angular/core';
import { AsyncPipe, DOCUMENT } from '@angular/common';
import { FormGroup, FormBuilder, Validators, ReactiveFormsModule, FormsModule } from '@angular/forms';
import { Observable, of} from 'rxjs';
import { map, debounceTime, distinctUntilChanged, switchMap, tap, filter } from 'rxjs/operators';
import { MatOption } from '@angular/material/core';
import { MatIconModule } from '@angular/material/icon';
import { MatFormFieldModule } from '@angular/material/form-field';
import { MatAutocompleteModule } from '@angular/material/autocomplete';
import { MatSelectModule } from '@angular/material/select';
import { MatTooltipModule } from '@angular/material/tooltip';
import { MatPaginator, MatPaginatorModule} from '@angular/material/paginator';

import { SpaceFields} from '../model';
import { User, UserFields } from '../../auth/model';
import { SpaceService } from '../service';
import { AuthService } from '../../auth/service';
import { UserInfo, Invitation, Space } from '../../api';

enum INVITATION_TYPE {
	MY_REQUEST = 0,
	I_WAS_INVITED = 1,
	WE_INVITED = 2,
	WANTS_TO_JOIN = 3,
}

type Detail = Invitation & SpaceFields & UserFields & { type: INVITATION_TYPE };

@Component({
	selector: 'app-space-invitation-list',
	templateUrl: './component.html',
	styleUrls: ['./component.css', '../common.css'],
	standalone: true,
	imports: [
		AsyncPipe, FormsModule, ReactiveFormsModule,
		MatIconModule, MatFormFieldModule, MatAutocompleteModule, MatSelectModule, MatTooltipModule, MatPaginatorModule,
	]
})
export class SpaceInvitationListComponent implements OnInit {
	private document = inject(DOCUMENT);
	private fb = inject(FormBuilder);
	private space = inject(SpaceService);
	private auth = inject(AuthService);

	TYPE = INVITATION_TYPE;

	form: FormGroup;
	isFormHidden: boolean = true;
	formSpaceId: string;
	formSpaceName: string;
	formUser: User;

	total: number;

	@Input() pageSize: number = 7;
	@Output() onPageSize = new EventEmitter<number>();
	@Output() onApprove = new EventEmitter<Detail>();

	// userId текущего залогиненного юзера
	currentUserId: string;

	_space: Space = null;
	@Input()
	set currentSpace(value: Space) {
		this._space = value;
		this.getItems(this.pageSize, 0);
	}
	get currentSpace() {
		return this._space;
	}

	items: Detail[] = [];

	users$: Observable<UserInfo[]>;
	hasUsers: boolean;

	@ViewChild('paginator') paginator: MatPaginator;

	constructor() {
		this._initForm();
	}

	ngOnInit() {
		this.currentUserId = this.auth.user.id;

		this.users$ = this.form.controls['login'].valueChanges.pipe(
			tap(value => {
				this.formUser = this.hasUsers = undefined;

				// если передан объект, значит пользователь выбрал элемент из списка
				if (typeof value === "object") {
					this.formUser = value;
				}
			}),
			filter(value => typeof value === "string"), // ищем только если передана строка
			debounceTime(300), // Optional: debounce input changes to avoid excessive requests
			distinctUntilChanged(), // Optional: ensure distinct values before making requests
			switchMap(value =>  this.auth.getList(10, 0, value).pipe(
				tap(res => this.hasUsers = res.length > 0)
			))
		);

		this.getItems(this.pageSize, 0);
	}

	getItems(limit: number, page: number) {
		if (this.pageSize != limit) {
			this.pageSize = limit;
			this.onPageSize.emit(limit);
		}

		// Если space уже установлен, но ещё не заполнены поля (не получен объект с сервера)
		if (this._space && !this._space.id)
			return;

		this.space.getInvitationList(limit, page, this._space?.id)
			.subscribe(res => {
				this.items = res.list as Detail[];
				this.total = res.total;
				this.fillType();
				this.auth.fillFields(this.items);

				if (!this._space?.id)
					this.space.fillFields(this.items);
			});
	}

	onDelBtn(item: Detail) {
		this.space.delInvitationById(item.id)
			.subscribe(_ => {
				this.goToFirstPage();
			});
	}

	private _initForm() {
		this.form = this.fb.group({
			login: ['', [
				Validators.required,
			]],
			role: ['', [
				Validators.required
			],],
		});
	}

	onAdd(space: Space) {
		this.formSpaceId = space.id;
		this.formSpaceName = space.name;
		this.isFormHidden = false;
	}

	onFormCloseBtn() {
		this.isFormHidden = true;
		this.formSpaceName = "";
		this.form.reset();
	}

	displayUserLogin(value) {
		return value?.login;
	}

	onSubmit(): void {
		if (this.form.invalid) {
			return;
		}
		if (!this.formUser) {
			return;
		}
		this.space.createInvitation(
			this.formSpaceId,
			this.formUser.id,
			this.form.value.role
		).subscribe(_ => {
			this.form.reset();
			this.isFormHidden = true;
			this.goToFirstPage();
		});
	}

	private fillType() {
		const itemType = item => {
			if (item.userId == this.currentUserId) {
				if (item.creatorId == item.userId)
					return INVITATION_TYPE.MY_REQUEST;
				else
					return INVITATION_TYPE.I_WAS_INVITED;
			} else {
				if (item.creatorId == item.userId)
					return INVITATION_TYPE.WANTS_TO_JOIN;
				else
					return INVITATION_TYPE.WE_INVITED;
			}
			return null;
		};

		for (const item of this.items) {
			item.type = itemType(item);
		}
	}

	changeRole(value, item: Detail) {
		if (item.type == INVITATION_TYPE.WE_INVITED || item.type == INVITATION_TYPE.WANTS_TO_JOIN) {
			this.space.changeRoleInInvitation(item.id, value)
				.subscribe(res => {
					if (res) {
						this.goToFirstPage();
					}
				})
		}
	}

	approveInvitation(item: Detail) {
		if (!item.roleId) {
			return;
		}
		this.space.approveInvitation(item.id)
			.subscribe(_ => {
				this.goToFirstPage();
				this.onApprove.emit(item);
			});
	}

	goToFirstPage() {
		if (this.paginator.pageIndex == 0) {
			this.getItems(this.pageSize, 0);
		} else {
			this.paginator.firstPage();
		}
	}
}

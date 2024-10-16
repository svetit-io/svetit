import { Component, OnInit, inject } from '@angular/core';
import { FormControl, ReactiveFormsModule } from '@angular/forms';
import { Observable, of } from 'rxjs';
import { map, debounceTime, distinctUntilChanged, switchMap, tap, filter } from 'rxjs/operators';
import { MatOption } from '@angular/material/core';
import { Router, NavigationExtras } from '@angular/router';
import { FormGroup, FormBuilder, Validators } from '@angular/forms';

import { SpaceService } from '../service';
import { AuthService } from '../../auth/service';
import { Space } from '../../api';
import { MatFormFieldModule } from '@angular/material/form-field';
import { AsyncPipe } from '@angular/common';
import { MatSelectModule } from '@angular/material/select';
import {MatAutocompleteModule} from '@angular/material/autocomplete';
import { MatCardModule } from '@angular/material/card';

@Component({
	selector: 'app-space-add',
	templateUrl: './component.html',
	styleUrls: ['./component.css', '../common.css'],
	standalone: true,
	imports: [
		AsyncPipe, ReactiveFormsModule, MatFormFieldModule, MatSelectModule, MatAutocompleteModule, MatCardModule,
	]
})
export class SpaceAddComponent implements OnInit {
	private router = inject(Router);
	private fb = inject(FormBuilder);
	private space = inject(SpaceService);
	private auth = inject(AuthService);

	createForm: FormGroup;
	spaceAutocomplete = new FormControl('');

	spaces$!: Observable<Space[]>;
	selectedSpace: Space;
	keyWasChanged: boolean = false;
	hasSpaces: boolean;

	// userId текущего залогиненного юзера
	currentUserId: string;

	constructor() {
		this._createForm();
	}

	ngOnInit() {
		this.currentUserId = this.auth.user.id;

		this.spaces$ = this.spaceAutocomplete.valueChanges.pipe(
			tap(value => {
				this.selectedSpace = this.hasSpaces = undefined;

				// если передан объект, значит пользователь выбрал элемент из списка
				if (typeof value === "object") {
					this.selectedSpace = value;
				}
			}),
			filter(value => typeof value === "string"), // ищем только если передана строка
			debounceTime(300), // Optional: debounce input changes to avoid excessive requests
			distinctUntilChanged(), // Optional: ensure distinct values before making requests
			switchMap(value => this.space.getAvailableList(10, 0, value).pipe(
				map(res => {
					this.hasSpaces = res.list.length > 0;
					return res.list;
				}))
			)
		);
	}

	sendRequestToJoin() {
		this.space.join(this.selectedSpace.id, this.currentUserId)
			.subscribe(res => {
				const navigationExtras: NavigationExtras = {state: {spaceName: this.selectedSpace.name}};
				this.router.navigate(['space/add/request'], navigationExtras);
			});
	}

	private translitFromRuToEn = {
		"ё": "yo", "й": "i", "ц": "ts", "у": "u", "к": "k", "е": "e", "н": "n", "г": "g",
		"ш": "sh", "щ": "sch", "з": "z", "х": "h", "ъ": "'", "ф": "f", "ы": "i", "в": "v",
		"а": "a", "п": "p", "р": "r", "о": "o", "л": "l", "д": "d", "ж": "zh", "э": "e",
		"я": "ya", "ч": "ch", "с": "s", "м": "m", "и": "i", "т": "t", "ь": "'", "б": "b","ю": "yu"
	};

	private _transliterate(word: string): string {
		return word
			.split('')
			.map(char => this.translitFromRuToEn[char] || char)
			.join('');
	}

	onNameChange(target: EventTarget) {
		if (this.keyWasChanged)
			return;

		let key = (target as HTMLInputElement).value.toLowerCase();
		key = this._transliterate(key);
		key = key.replace(/[^a-z0-9_]/g, '');
		this.createForm.patchValue({key});
	}

	onKeyChange() {
		this.keyWasChanged = true;
	}

	onSubmitCreate(data): void {
		if (this.createForm.invalid) {
			return;
		}
		this.space.createNew(data.name, data.key, data.requestsAllowed)
			.subscribe(_ => this.router.navigate(['space/list']));
	}

	private _createForm() {
		this.createForm = this.fb.group({
			name: ['', [Validators.required]],
			key: ['', [
				Validators.required,
				Validators.pattern('[a-z0-9_]*'),
			]],
			requestsAllowed: [false, [Validators.required]],
		});
	}

	displaySpaceName(value) {
		return value?.name;
	}
}

import { Component, OnInit } from '@angular/core';
import { FormControl } from '@angular/forms';
import { Observable } from 'rxjs';
import { startWith, map, debounceTime, distinctUntilChanged, switchMap, tap } from 'rxjs/operators';
import { MatOption } from '@angular/material/core';
import { Router, NavigationExtras } from '@angular/router';
import { FormGroup, FormBuilder, Validators } from '@angular/forms';

import { SpaceService } from '../service';
import { UserService } from '../../user/service';
import { Space } from '../model';

@Component({
	selector: 'app-space-add',
	templateUrl: './component.html',
	styleUrls: ['./component.css', '../common.css'],
})
export class SpaceAddComponent implements OnInit {
	createForm: FormGroup;
	spaceAutocomplete = new FormControl('');

	spaces$!: Observable<Space[]>;
	selectedSpace: Space;
	keyWasChanged: boolean = false;
	hasSpaces: boolean;

	// userId текущего залогиненного юзера
	currentUserId: string;

	constructor(
		private router: Router,
		private fb: FormBuilder,
		private space: SpaceService,
		private user: UserService,
	) {
		this._createForm();
	}

	ngOnInit() {
		this.currentUserId = this.user.info.id;

		this.spaces$ = this.spaceAutocomplete.valueChanges.pipe(
			tap(_ => this.hasSpaces = false),
			startWith(''),
			debounceTime(300), // Optional: debounce input changes to avoid excessive requests
			distinctUntilChanged(), // Optional: ensure distinct values before making requests
			switchMap(value => {
				if (!value)
					this.selectedSpace = null;
				return this.space.getAvailableList(10, 0, value || '', this.currentUserId).pipe(
					map(res => {
							this.hasSpaces = res.results.length > 0;
							return res.results;
					})
				);
			})
		);
	}

	onSelectOption(option: MatOption) {
		if (option?.value) {
			this.selectedSpace = option.value;
		}
	}

	onAutoInputChange(name: string) {
		if (this.selectedSpace?.name != name)
			this.selectedSpace = null;
	}

	sendRequestToJoin() {
		this.space.join(this.selectedSpace.id)
			.subscribe(res => {
				if (res) {
					const navigationExtras: NavigationExtras = {state: {spaceName: this.selectedSpace.name}};
					this.router.navigate(['space/add/request'], navigationExtras);
				}
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

	onNameChange(name: string) {
		if (!this.keyWasChanged) {
			let newKey = name.toLowerCase();
			newKey = this._transliterate(newKey);
			newKey = newKey.replace(/[^a-z0-9_]/g, '');
			this.createForm.patchValue({
				key: newKey
			});
		}
	}

	onKeyChange(key: string) {
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

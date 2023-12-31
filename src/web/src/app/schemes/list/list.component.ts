import {ChangeDetectorRef, Component, OnDestroy, OnInit, QueryList, ViewChild, ViewChildren} from '@angular/core';
import { Router } from '@angular/router';
import { HttpClient } from '@angular/common/http';
import { PageEvent } from '@angular/material/paginator';
import { MatPaginator } from '@angular/material/paginator';
import { MatDialog } from '@angular/material/dialog';
import { TranslateService } from '@ngx-translate/core';

import { of, Subject, Subscription } from 'rxjs';
import { debounceTime, distinctUntilChanged, switchMap } from 'rxjs/operators';

import { Scheme} from '../../user';
import { SchemesService } from '../schemes.service';
import { UserService } from '../../user/service';
import { Create_Scheme_Dialog } from './create-scheme-dialog/create-scheme-dialog';
import {SchemesList} from '../schemes-list';

@Component({
	selector: 'app-schemes',
	templateUrl: './list.component.html',
	styleUrls: ['./list.component.css', '../schemes-list.css']
})
export class SchemeListComponent extends SchemesList implements OnInit, OnDestroy {
	timeout: any;
	start = 0;
	limit = 10;

	get isExtraList(): boolean {
		return this.user.isExtraList();
	}

	constructor(
		private router: Router,
		private schemesService: SchemesService,
		private user: UserService,
		http: HttpClient,
		translate: TranslateService,
		public dialog: MatDialog,
		private changeDetectorRef: ChangeDetectorRef,
	) {
			super(http, translate);
	}

	searchString: Subject<string> = new Subject<string>();

	schemesSubs: Subscription;

	searchQ: Subject<string>;

	resultsLength = 0;

	@ViewChildren(MatPaginator) paginator: QueryList<MatPaginator>;
	citySelected = null;
	cities: any[];
	compSelected: any;
	comps: any[];
	pageEvent: PageEvent;

	@ViewChild('searchBox', {static: true}) searchBox;

	ngOnInit() {
		this.getSchemes();

		this.schemesService.getCities().subscribe(data => {
			this.cities = data.results;
		});

		this.schemesService.getCompanies().subscribe(data => {
			this.comps = data.results;
		});

		this.searchString.pipe(
			debounceTime(400),
			distinctUntilChanged(),
			switchMap(text => {
				this.search(text);
				return of(text);
			})
		).subscribe(response => {

		});
	}

	search(value: string) {
		let v = value;

		if (this.citySelected) {
			v += '&city__id=' + this.citySelected;
		}

		if (this.compSelected) {
			v += '&company__id=' + this.compSelected;
		}

		this.getSchemes(v);
	}

	getSchemes(query: string = ''): void {
		this.ngOnDestroy();

		if (this.schemesSubs) {
			this.schemesSubs.unsubscribe();
		}

		if (this.statusItemSubs) {
			this.statusItemSubs.map(ss => ss.unsubscribe());
		}

		if (this.timeout) {
			clearTimeout(this.timeout);
		}

		if (this.paginator) {
			console.log(this.paginator);
		}

		this.schemesSubs = this.schemesService.getSchemes(this.limit, this.start, 'title',
			query)
			.subscribe(dat => {
				console.log(dat);
				this.resultsLength = dat.count;
				this.schemes = dat.results;

				//console.log(this.schemes);
				this.timeout = setTimeout(() => this.getStatuses(), 1000);

				this.schemesSubs.unsubscribe();
			});
	}

	delete(scheme: Scheme): void {
		this.schemes = this.schemes.filter(h => h !== scheme);
		this.schemesService.deleteScheme(scheme).subscribe();
	}

	detail(scheme: Scheme): void {
		this.router.navigate([`/detail/${scheme.id}/`]);
	}

	getPaginatorData(event: PageEvent) {
		console.log(event);

		const q = this.searchBox.nativeElement.value;

		this.limit = event.pageSize;
		this.start = event.pageIndex;

		console.log(q);
		this.search(q);

		this.changeDetectorRef.detectChanges();
		return event;
	}

	open_create_scheme_dialog(): void
	{
		const dialogRef = this.dialog.open(Create_Scheme_Dialog, { width: '80%', data: { cities: this.cities, comps: this.comps } });

		dialogRef.afterClosed().subscribe(new_scheme => {
			if (!new_scheme)
				return;

			this.schemesService.create_scheme(new_scheme)
				.subscribe(scheme => {
					if (scheme)
						this.schemes.push(scheme);
				});
		});
	}
}

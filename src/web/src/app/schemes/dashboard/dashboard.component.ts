import {Component, OnDestroy, OnInit} from '@angular/core';
import {Router} from '@angular/router';

import {SchemesService} from '../schemes.service';
import {FavService} from '../../fav.service';
import {TranslateService} from '@ngx-translate/core';
import {HttpClient} from '@angular/common/http';
import {SchemesList} from '../schemes-list';
import {Scheme} from '../../user';
import {combineLatest, concat} from 'rxjs';

@Component({
	selector: 'app-dashboard',
	templateUrl: './dashboard.component.html',
	styleUrls: ['./dashboard.component.css', '../../sections.css', '../schemes-list.css']
})
export class DashboardComponent extends SchemesList implements OnInit, OnDestroy {
	favschemes: Scheme[];

	constructor(
		private router: Router,
		private schemesService: SchemesService,
		private favService: FavService,
		http: HttpClient,
		translate: TranslateService,
	) {
		super(http, translate);
	}

	ngOnInit() {
		this.getSchemes();
	}

	getSchemes(): void {
		this.schemesService.getSchemes(5, 0, '-last_usage')
			.subscribe(data => {
				this.schemes = data.results.slice(0, 5);
				this.getStatuses();
				this.getFavSchemes();
			});
	}

	getFavSchemes(): void {
		this.favschemes = this.favService.getFavs() as Scheme[]; // preloading list from cookies

		const observables = this.favschemes.map(schemeInfo => this.schemesService.getScheme(schemeInfo.name));
		combineLatest(observables).subscribe(schemes => {
			this.favschemes = schemes;
			this.getStatuses(this.favschemes);
		});
	}
}

import { ChangeDetectorRef, Component, OnInit, OnDestroy, inject } from '@angular/core';
import {
	Router, Event as RouterEvent, ActivatedRoute,
	NavigationStart,
	NavigationEnd,
	NavigationCancel,
	NavigationError,
    RouterOutlet
} from '@angular/router';

import {MediaMatcher} from '@angular/cdk/layout';
import {Title} from '@angular/platform-browser';
import {TranslateService} from '@ngx-translate/core';
import {CookieService} from 'ngx-cookie-service';
import {Subscription} from 'rxjs';

import { FormsModule } from '@angular/forms';
import {MatButtonModule} from '@angular/material/button';
import {MatIconModule} from '@angular/material/icon';
import {MatSelectModule} from '@angular/material/select';
import {MatProgressSpinnerModule} from '@angular/material/progress-spinner';

import {AuthService} from './auth/service';
import {SpaceService} from './space/service';
import { UserBadgeService } from './user-badge/service';

import {UIService} from './ui.service';

import { SpaceListComponent } from './space/list/component';
import { UserBadgeComponent } from './user-badge/component';
import { ProgressSpinnerComponent } from './request-watcher/progress-spinner/component';

@Component({
	selector: 'app-root',
	templateUrl: './app.component.html',
	styleUrls: ['./app.component.css'],
	standalone: true,
	imports: [
		RouterOutlet, FormsModule,
		MatButtonModule, MatIconModule, MatSelectModule, MatProgressSpinnerModule,
		UserBadgeComponent, ProgressSpinnerComponent,
	]
})
export class AppComponent implements OnInit, OnDestroy {
	translate = inject(TranslateService);
	private route = inject(ActivatedRoute);
	private router = inject(Router);
	uiService = inject(UIService);
	cookie = inject(CookieService);
	private changeDetectorRef = inject(ChangeDetectorRef);
	private title = inject(Title);
	private space = inject(SpaceService);
	private auth = inject(AuthService);
	private userBadges = inject(UserBadgeService);

	loading = true;

	mobileQuery: MediaQueryList;
	private _mobileQueryListener: () => void;

	scrollTop = 0;

	languages = [
		{code: 'en', label: 'English', icon: 'fi fi-gb'},
		{code: 'ru', label: 'Русский', icon: 'fi fi-ru'},
		// { code: 'es', label: 'Español', icon: 'fi fi-es'},
	];

	current_lang_: any;
	cookieGot: boolean;
	showDropDown = false;
	authorized: boolean;
	initialized: boolean;
	private title$: Subscription;
	private _subAuth: Subscription;
	private _subSpace: Subscription;
	private _subSpaceEvent: Subscription;

	get isAdmin(): boolean {
		return this.auth.isAdmin();
	}

	constructor() {
		const translate = this.translate;
		const cookie = this.cookie;
		const changeDetectorRef = this.changeDetectorRef;
		const media = inject(MediaMatcher);

		this.cookieGot = this.cookie.get('cookie-agree') === 'true';

		this.router.events.subscribe((event: RouterEvent) => this.navigationInterceptor(event));

		this.mobileQuery = media.matchMedia('(max-width: 600px)');
		this._mobileQueryListener = () => changeDetectorRef.detectChanges();
		this.mobileQuery.addListener(this._mobileQueryListener);

		translate.addLangs(['en', 'ru']);
		// this language will be used as a fallback when a translation isn't found in the current language
		translate.setDefaultLang('en');
		// the lang to use, if the lang isn't available, it will use the current loader to get them
		// translate.use('ru');

		let lang = cookie.get('lang');
		if (!lang) {
			const browserLang = translate.getBrowserLang();
			console.log('Browser Lang:' + browserLang);

			lang = browserLang.match(/ru|en/) ? browserLang : 'en';
			this.cookie.set('lang', lang, 365, '/');
		}

		const fileLang = document.getElementsByTagName('html')[0].getAttribute('lang');
		if (fileLang != lang) {
			const url = new URL(window.location.href);
			url.searchParams.set('lang', lang);
			window.location.href = url.toString();
			return;
		}

		for (const item of this.languages) {
			if (item.code == lang) {
				this.current_lang_ = item;
				break;
			}
		}

		console.log("Lang:", lang, "current_lang_:", this.current_lang_);

		translate.use(lang);
		this.uiService.setCurLang(lang);
	}

	set_language() {
		this.cookie.set('lang', this.current_lang_.code, 365, '/');
		const url = new URL(window.location.href);
		url.searchParams.set('lang', this.current_lang_.code);
		window.location.href = url.toString();
	}

	ngOnInit() {
		this._subAuth = this.auth.isAuthorized().subscribe(ok => {
			this.authorized = ok;
			this.changeDetectorRef.detectChanges();
		});

		this._subSpace = this.space.isInitialized().subscribe(res => {
			this.initialized = true;

			const invitationSize = res.invitationSize;
			this.userBadges.spaceInvitationSize = invitationSize;
			this.userBadges.userNotificationSize += invitationSize;

			this.changeDetectorRef.detectChanges();
		});
	}

	ngOnDestroy(): void {
		this._subSpace.unsubscribe();
		this._subSpaceEvent.unsubscribe();
		this._subAuth.unsubscribe();
		this.title$.unsubscribe();
		this.mobileQuery.removeListener(this._mobileQueryListener);
	}

	navigationInterceptor(event: RouterEvent): void {
		if (event instanceof NavigationStart) {
			this.loading = true;
		}
		if (event instanceof NavigationEnd) {
			this.loading = false;
		}

		// Set loading state to false in both of the below events to hide the spinner in case a request fails
		if (event instanceof NavigationCancel) {
			this.loading = false;
		}
		if (event instanceof NavigationError) {
			this.loading = false;
		}
		this.changeDetectorRef.detectChanges();
	}

	onScroll($event) {
		this.scrollTop = $event.target.scrollTop;
	}

	cookieAgree() {
		this.cookie.set('cookie-agree', 'true', 365, '/');
		this.cookieGot = true;
	}

	toggleDropDown() {
		this.showDropDown = !this.showDropDown;
	}
}

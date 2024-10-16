import { NgModule } from '@angular/core';
import { CommonModule } from '@angular/common';
import { FormsModule, ReactiveFormsModule } from '@angular/forms';


import {UserSettingsComponent} from './user-settings.component';
import {UserSettingsRoutingModule} from './user-settings-routing.module';
import {NotificationsComponent} from './notifications/notifications.component';

import { HttpClient } from '@angular/common/http';
import {TranslateModule, TranslateLoader, TranslateService} from '@ngx-translate/core';
import { TranslateHttpLoader } from '@ngx-translate/http-loader';
import { UserDetailsComponent } from './user-details/user-details.component';
import {ISchemeService} from '../ischeme.service';
import { SchemeGroupsComponent } from './scheme-groups/scheme-groups.component';
import {SchemeGroupsModule} from '../scheme-groups/scheme-groups.module';

export function HttpLoaderFactory(httpClient: HttpClient) {
  return new TranslateHttpLoader(httpClient, './assets/i18n/', '.json');
}

@NgModule({
    imports: [
    CommonModule,
    FormsModule,
    ReactiveFormsModule,
    UserSettingsRoutingModule,
    TranslateModule.forChild({
        loader: {
            provide: TranslateLoader,
            useFactory: HttpLoaderFactory,
            deps: [HttpClient]
        }
    }),
    SchemeGroupsModule,
    UserSettingsComponent,
    NotificationsComponent,
    UserDetailsComponent,
    SchemeGroupsComponent
],
    providers: [
        ISchemeService
    ]
})
export class UserSettingsModule {
}

import {Component, OnInit} from '@angular/core';
import {SchemesService} from '../../schemes/schemes.service';
import {Scheme_Group} from '../../user';

import { RouterLinkActive, RouterLink } from '@angular/router';

@Component({
    selector: 'app-scheme-groups-list',
    templateUrl: './scheme-groups-list.component.html',
    styleUrls: ['./scheme-groups-list.component.css'],
    standalone: true,
    imports: [RouterLinkActive, RouterLink]
})
export class SchemeGroupsListComponent implements OnInit {
    schemeGroups: Scheme_Group[];

    constructor(private schemes: SchemesService) {
    }

    ngOnInit(): void {
        this.schemes.get_scheme_groups().subscribe(groups => this.schemeGroups = groups);
    }
}

import {Component, OnInit} from '@angular/core';
import {ActivatedRoute} from '@angular/router';

@Component({
	selector: 'app-temp',
	templateUrl: './component.html',
	styleUrls: ['./component.css']
})
export class TempSpaceComponent implements OnInit {

	constructor(
		private route: ActivatedRoute,
	) {
	}

	ngOnInit() {
	}
}

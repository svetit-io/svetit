import { ComponentFixture, TestBed } from '@angular/core/testing';

import { ElementsMenuComponent } from './elements-menu.component';

describe('ElementsMenuComponent', () => {
  let component: ElementsMenuComponent;
  let fixture: ComponentFixture<ElementsMenuComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
    imports: [ElementsMenuComponent]
})
    .compileComponents();
  });

  beforeEach(() => {
    fixture = TestBed.createComponent(ElementsMenuComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});

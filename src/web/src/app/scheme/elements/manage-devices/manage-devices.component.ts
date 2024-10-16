import { Component, OnInit, inject } from '@angular/core';
import {AuthService} from '../../../auth/service';
import {SchemeService} from '../../scheme.service';
import {Device, Device_Item} from '../../scheme';
import {MatDialog} from '@angular/material/dialog';
import {DeviceDetailDialogComponent} from '../manage/device-detail-dialog/device-detail-dialog.component';
import {DeviceItemDetailDialogComponent} from '../manage/device-item-detail-dialog/device-item-detail-dialog.component';
import {UIService} from '../../../ui.service';
import {Structure_Type} from '../../settings/settings';
import {SidebarService} from '../../sidebar.service';
import {EditorModeFromSidebar} from '../../editor-mode-from-sidebar';
import { SchemeSectionComponent } from '../../scheme-section/scheme-section.component';

import { MatIconButton, MatButton } from '@angular/material/button';
import { MatIcon } from '@angular/material/icon';
import { DevItemValueComponent } from '../../dev-item-value/dev-item-value.component';

@Component({
    selector: 'app-manage-devices',
    templateUrl: './manage-devices.component.html',
    styleUrls: ['./manage-devices.component.css', '../manage/manage.component.css', '../../device-item-group/device-item-group.component.css'],
    standalone: true,
    imports: [SchemeSectionComponent, MatIconButton, MatIcon, DevItemValueComponent, MatButton]
})
export class ManageDevicesComponent extends EditorModeFromSidebar implements OnInit {
    private schemeService = inject(SchemeService);
    private dialog = inject(MatDialog);
    private ui = inject(UIService);

    devices: Device[];

    constructor() {
        const sidebar = inject(SidebarService);

        super(sidebar);
        this.devices = this.schemeService.scheme.device;
    }

    isDisabled(): boolean {
        return !this.schemeService.isSchemeConnected;
    }

    ngOnInit(): void {
        super.init();
    }

    editDevice(device: Device) {
        this.dialog.open(DeviceDetailDialogComponent, { data: device, width: '450px' })
            .afterClosed()
            .subscribe((newDevice: Device) => {});
    }

    removeDevice(device: Device) { // TODO: refactor remove methods below to use confirmationDialog().pipe() instead of nested subscribe
        this.ui.confirmationDialog()
            .subscribe((confirmation: boolean) => {
                if (!confirmation) return;

                this.schemeService.remove_structure(Structure_Type.ST_DEVICE, device)
                    .subscribe(() => {});
            });
    }

    editItem(item: Device_Item) {
        this.dialog.open(DeviceItemDetailDialogComponent, {
            data: {
                ...item,
                disableDeviceIdChanging: true,
            },
            width: '450px',
        })
            .afterClosed()
            .subscribe((updatedItem: Device_Item) => {});
    }

    removeItem(item: Device_Item) {
        this.ui.confirmationDialog()
            .subscribe((confirmation) => {
                if (!confirmation) return;

                this.schemeService.remove_structure(Structure_Type.ST_DEVICE_ITEM, item)
                    .subscribe(() => {});
            });
    }

    newItem(device: Device) {
        this.dialog.open(DeviceItemDetailDialogComponent, {
            width: '80%',
            data: {
                device_id: device.id,
                disableDeviceIdChanging: true,
            },
        })
            .afterClosed()
            .subscribe((newItem: Device_Item) => {});
    }

    newDevice() {
        this.dialog.open(DeviceDetailDialogComponent, { width: '80%' })
            .afterClosed()
            .subscribe((device: Device) => {});
    }
}

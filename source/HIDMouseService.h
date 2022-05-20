#ifndef BLE_HID_MOUSE_SERVICE_H__
#define BLE_HID_MOUSE_SERVICE_H__

#if BLE_FEATURE_GATT_SERVER

#include "ble/BLE.h"
#include "ble/Gap.h"
#include "ble/GattServer.h"
#include "HIDService.h"

namespace
{

// Input Report.
#pragma pack(push, 1)
    struct
    {
        uint8_t buttons;
        uint8_t dx;
        uint8_t dy;
    } hid_input_report;
#pragma pack(pop)

    // Input report reference.
    static report_reference_t input_report_ref = {0, INPUT_REPORT};

    static GattAttribute input_report_ref_desc(
        ATT_UUID_HID_REPORT_ID_MAPPING,
        (uint8_t *)&input_report_ref,
        sizeof(input_report_ref),
        sizeof(input_report_ref));

    static GattAttribute *input_report_ref_descs[] = {
        &input_report_ref_desc,
    };

    static uint8_t ReportDescriptor[] ={
        0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
        0x09, 0x02,                    // USAGE (Mouse)
        0xa1, 0x01,                    // COLLECTION (Application)
        0x09, 0x01,                    //   USAGE (Pointer)
        0xa1, 0x00,                    //   COLLECTION (Physical)
        0x05, 0x09,                    //     USAGE_PAGE (Button)
        0x19, 0x01,                    //     USAGE_MINIMUM (Button 1)
        0x29, 0x03,                    //     USAGE_MAXIMUM (Button 3)
        0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
        0x25, 0x01,                    //     LOGICAL_MAXIMUM (1)
        0x95, 0x03,                    //     REPORT_COUNT (3)
        0x75, 0x01,                    //     REPORT_SIZE (1)
        0x81, 0x02,                    //     INPUT (Data,Var,Abs)
        0x95, 0x01,                    //     REPORT_COUNT (1)
        0x75, 0x05,                    //     REPORT_SIZE (5)
        0x81, 0x03,                    //     INPUT (Cnst,Var,Abs)
        0x05, 0x01,                    //     USAGE_PAGE (Generic Desktop)
        0x09, 0x30,                    //     USAGE (X)
        0x09, 0x31,                    //     USAGE (Y)
        0x15, 0x81,                    //     LOGICAL_MINIMUM (-127)
        0x25, 0x7f,                    //     LOGICAL_MAXIMUM (127)
        0x75, 0x08,                    //     REPORT_SIZE (8)
        0x95, 0x02,                    //     REPORT_COUNT (2)
        0x81, 0x06,                    //     INPUT (Data,Var,Rel)
        0xc0,                          //   END_COLLECTION
        0xc0                           // END_COLLECTION
    };

} // namespace ""

/**
 * BLE HID Mouse Service
 *
 * @par usage
 *
 * When this class is instantiated, it adds a mouse HID service in
 * the GattServer.
 *
 * @attention Multiple instances of this hid service are not supported.
 * @see HIDService
 */

class HIDMouseService : public HIDService
{
public:
    enum Button
    {
        BUTTON_NONE = 0,
        BUTTON_LEFT = 1 << 0,
        BUTTON_RIGHT = 1 << 1,
        BUTTON_MIDDLE = 1 << 2,
    };

    HIDMouseService(BLE &_ble) : HIDService(_ble,
                                            HID_MOUSE,

                                            // report map
                                            ReportDescriptor,
                                            sizeof(ReportDescriptor) / sizeof(*ReportDescriptor),

                                            // input report
                                            (uint8_t *)&hid_input_report,
                                            sizeof(hid_input_report),
                                            input_report_ref_descs,
                                            sizeof(input_report_ref_descs) / sizeof(*input_report_ref_descs))
    {
    }

    ble::adv_data_appearance_t appearance() const override
    {
        return ble::adv_data_appearance_t::MOUSE;
    }

    void motion(float fx, float fy)
    {
        hid_input_report.dx = static_cast<uint8_t>(0x100 + fx * 0x7f) & 0xff;
        hid_input_report.dy = static_cast<uint8_t>(0x100 + fy * 0x7f) & 0xff;
    }

    void button(Button buttons)
    {
        hid_input_report.buttons = static_cast<uint8_t>(buttons);
    }
};

#endif // BLE_FEATURE_GATT_SERVER

#endif // BLE_HID_MOUSE_SERVICE_H__

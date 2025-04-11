#include <napi.h>
#include <windows.h>
#include <string>

std::string GetDriveTypeString(UINT driveType) {
    switch (driveType) {
        case DRIVE_UNKNOWN: return "Unknown";
        case DRIVE_NO_ROOT_DIR: return "Invalid Path";
        case DRIVE_REMOVABLE: return "Removable";
        case DRIVE_FIXED: return "Fixed";
        case DRIVE_REMOTE: return "Network";
        case DRIVE_CDROM: return "CD-ROM";
        case DRIVE_RAMDISK: return "RAM Disk";
        default: return "Unknown";
    }
}

Napi::Array GetDrives(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    Napi::Array drivesArray = Napi::Array::New(env);

    DWORD drives = GetLogicalDrives();
    if (drives == 0) {
        Napi::Error::New(env, "GetLogicalDrives failed").ThrowAsJavaScriptException();
        return drivesArray;
    }

    uint32_t index = 0;
    for (char i = 0; i < 26; ++i) {
        if (drives & (1 << i)) {
            char driveLetter = 'A' + i;
            std::string rootPath = std::string(1, driveLetter) + ":\\";
            UINT type = GetDriveTypeA(rootPath.c_str());

            Napi::Object driveObj = Napi::Object::New(env);
            driveObj.Set("letter", rootPath);
            driveObj.Set("type", GetDriveTypeString(type));
            drivesArray.Set(index++, driveObj);
        }
    }

    return drivesArray;
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
    exports.Set("getDrives", Napi::Function::New(env, GetDrives));
    return exports;
}

NODE_API_MODULE(drive_info, Init)

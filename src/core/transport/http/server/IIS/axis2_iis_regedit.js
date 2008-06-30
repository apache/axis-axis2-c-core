var WshShell = WScript.CreateObject("WScript.Shell");

/* You can change the following values to suite your requirements */
/* Axis2/C repo location. axis2.xml, modules folder and services folder should be in this dir */
var axis2c_home             = WshShell.CurrentDirectory;
/* Log level. Possible values are trace, error, info, critical, user, debug and warning */
var log_level               = "debug";
/* Full path to the log file */
var log_file                = axis2c_home + "/logs/axis2.log";
/* Services URL prefix. This is the folder where services are hosted. Optional */
var services_url_prefix;
/* Max log file size */
var max_log_file_size;
/* Axis2 location */
var axis2_location;

/* Don't change anything below */
var axis2c_home_str         = "axis2c_home";
var log_level_str           = "log_level";
var log_file_str            = "log_file";
var services_url_prefix_str = "services_url_prefix";
var max_log_file_size_str   = "max_log_file_size";
var axis2_location_str      = "axis2_location"

var reg_location            = "HKLM\\Software\\Apache Axis2C\\IIS ISAPI Redirector\\"
/* If specified get the values from the command prompt */
var args = WScript.Arguments;
if (args.length > 0) {
    axis2c_home = args.Item(0);
    if (axis2c_home) {
            log_file = axis2c_home + "/logs/axis2.log";
    }
}
if (args.length > 1) {
    log_level = args.Item(1);
}
if (args.length > 2) {
    log_file = args.Item(2);
}
if (args.length > 3) {
    services_url_prefix = args.Item(3);
}
if (args.length > 4) {
    max_log_file_size = args.Item(4);
}
if (args.length > 5) {
    axis2_location = args.Item(5);
}
/* Write the axis2c_home entry. This is used by Axis2/C to find the repo location */
WshShell.RegWrite (reg_location + axis2c_home_str, axis2c_home, "REG_SZ");
/* Write the log_level registry entry */
WshShell.RegWrite (reg_location + log_level_str, log_level, "REG_SZ");
/* Write the log file name */
WshShell.RegWrite (reg_location + log_file_str, log_file, "REG_SZ");
/* Write the services url prefix. We write this only if specified */
try {
    var services_url_prefix_key = WshShell.RegRead (reg_location + services_url_prefix_str);
    if (services_url_prefix_key) {
        WshShell.RegDelete (reg_location + services_url_prefix_str);    
    }
} catch (e) {}
if (services_url_prefix) {
    WshShell.RegWrite (reg_location + services_url_prefix_str, services_url_prefix, "REG_SZ");
}
/* We write max_log_file_size only if specified */
try {
    var max_log_file_size_key = WshShell.RegRead (reg_location + max_log_file_size_str);
    if (max_log_file_size_key) {
        WshShell.RegDelete (reg_location + max_log_file_size_str);    
    }
} catch (e) {}
if (max_log_file_size) {
    WshShell.RegWrite (reg_location + max_log_file_size_str, max_log_file_size, "REG_SZ");
}

try{
    var axis2_location_key = WshShell.RegRead (reg_location + axis2_location_str);
    if (axis2_location_key) {
            WshShell.RegDelete (reg_location + axis2_location_str);    
    }
} catch (e) {}
if (axis2_location) {
    WshShell.RegWrite (reg_location + axis2_location_str, axis2_location, "REG_SZ");
}
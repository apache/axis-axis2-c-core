/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements. See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * @file camera_control_service.h
 * @brief Apache Axis2/C HTTP/2 Generic Camera Control Service Header
 *
 * Generic camera control service using json-c library (no XML/SOAP dependencies).
 * Provides stub implementations for common camera operations that users can
 * customize for their specific camera systems (OpenCamera, V4L2, IP cameras, etc.).
 *
 * Features:
 * - Generic camera operation interface (start/stop recording, status, configure)
 * - HTTP/2 transport optimization
 * - Pure JSON processing (no axiom/XML)
 * - User-implementable stub functions for camera-specific integration
 * - Cross-platform camera control abstractions
 *
 * IMPLEMENTATION NOTE:
 * This service provides STUB implementations only. Users must implement the
 * camera-specific integration functions for their target camera system.
 * See the implementation guide for examples of integrating with:
 * - OpenCamera (Android JNI bridge)
 * - V4L2 (Linux Video4Linux2)
 * - IP cameras (RTSP/HTTP control)
 * - Custom camera hardware
 */

#ifndef CAMERA_CONTROL_SERVICE_H
#define CAMERA_CONTROL_SERVICE_H

#include <axutil_env.h>
#include <axutil_log_default.h>
#include <axutil_error_default.h>
#include <axutil_string.h>
#include <json-c/json.h>

#ifdef __cplusplus
extern "C"
{
#endif

/* Constants for camera control service */
#define CAMERA_CLIP_NAME_MAX_LENGTH    256
#define CAMERA_QUALITY_MAX_LENGTH      32
#define CAMERA_SETTINGS_MAX_LENGTH     1024
#define CAMERA_STATUS_MAX_LENGTH       512
#define CAMERA_ERROR_MSG_MAX_LENGTH    256
#define CAMERA_HOSTNAME_MAX_LENGTH     256
#define CAMERA_PATH_MAX_LENGTH         512
#define CAMERA_USERNAME_MAX_LENGTH     64
#define CAMERA_ID_MAX_LENGTH           64
#define CAMERA_TIMECODE_MAX_LENGTH     32
#define MAX_CAMERA_ENDPOINTS           256

/**
 * Camera recording parameters structure
 */
typedef struct camera_recording_params
{
    axis2_char_t *clip_name;     /**< Output clip/file name */
    axis2_char_t *quality;       /**< Quality setting (4K, 1080p, 720p, etc.) */
    int duration;                /**< Recording duration in seconds (0 = indefinite) */
    axis2_char_t *format;        /**< Video format (mp4, avi, mkv, etc.) */
} camera_recording_params_t;

/**
 * Camera settings structure
 */
typedef struct camera_settings
{
    axis2_char_t *resolution;    /**< Camera resolution */
    axis2_char_t *fps;          /**< Frames per second */
    axis2_char_t *codec;        /**< Video codec */
    axis2_char_t *bitrate;      /**< Video bitrate */
    axis2_char_t *audio_source; /**< Audio input source */
} camera_settings_t;

/**
 * Camera status structure
 */
typedef struct camera_status
{
    axis2_char_t *state;        /**< Current state (idle, recording, error) */
    axis2_char_t *active_clip;  /**< Currently recording clip name */
    int recording_duration;     /**< Current recording duration in seconds */
    axis2_char_t *last_error;   /**< Last error message */
} camera_status_t;

/**
 * SFTP transfer parameters structure
 */
typedef struct sftp_transfer_params
{
    axis2_char_t *hostname;         /**< Remote SFTP server hostname/IP */
    axis2_char_t *username;         /**< SSH username for authentication */
    axis2_char_t *private_key_path; /**< Path to SSH private key file */
    axis2_char_t *remote_path;      /**< Remote destination directory */
    axis2_char_t *local_file_path;  /**< Local file to transfer */
    int port;                       /**< SSH port (default 22) */
} sftp_transfer_params_t;

/**
 * Multi-camera deployment parameters structure
 *
 * IMPORTANT: Each camera is a STATELESS unit with its own IP address.
 * SMPTE timecode sync is handled externally via Tentacle Sync devices
 * that "jam sync" together and embed timecode on audio channel 1.
 *
 * Cameras do NOT coordinate with each other - sync is resolved in post-production.
 * This structure is for deployment convenience only, not runtime coordination.
 */
typedef struct multi_camera_deploy_params
{
    axis2_char_t *camera_id;        /**< Camera identifier for deployment (A-cam, B-cam, C-cam, etc.) */
    axis2_char_t *clip_prefix;      /**< Clip naming prefix for this camera */
    axis2_char_t *quality;          /**< Recording quality (4K, 1080p, etc.) */
    int duration;                   /**< Recording duration in seconds */
    axis2_char_t *format;           /**< Video format (mp4, mov, etc.) */
} multi_camera_deploy_params_t;

/* HTTP/2 Pure JSON service functions - No SOAP skeleton required */
AXIS2_EXTERN json_object* AXIS2_CALL
camera_control_service_invoke_json(const axutil_env_t *env,
                                  json_object *json_request);

/* Request/Response creation functions */
AXIS2_EXTERN camera_recording_params_t* AXIS2_CALL
camera_recording_params_create_from_json(const axutil_env_t *env,
                                        const axis2_char_t *json_string);

AXIS2_EXTERN camera_settings_t* AXIS2_CALL
camera_settings_create_from_json(const axutil_env_t *env,
                                const axis2_char_t *json_string);

AXIS2_EXTERN void AXIS2_CALL
camera_recording_params_free(camera_recording_params_t *params,
                            const axutil_env_t *env);

AXIS2_EXTERN void AXIS2_CALL
camera_settings_free(camera_settings_t *settings,
                    const axutil_env_t *env);

AXIS2_EXTERN void AXIS2_CALL
camera_status_free(camera_status_t *status,
                  const axutil_env_t *env);

AXIS2_EXTERN sftp_transfer_params_t* AXIS2_CALL
sftp_transfer_params_create_from_json(const axutil_env_t *env,
                                     const axis2_char_t *json_string);

AXIS2_EXTERN void AXIS2_CALL
sftp_transfer_params_free(sftp_transfer_params_t *params,
                         const axutil_env_t *env);

/* Multi-camera deployment functions */
AXIS2_EXTERN multi_camera_deploy_params_t* AXIS2_CALL
multi_camera_deploy_params_create_from_json(const axutil_env_t *env,
                                            const axis2_char_t *json_string);

AXIS2_EXTERN void AXIS2_CALL
multi_camera_deploy_params_free(multi_camera_deploy_params_t *params,
                                const axutil_env_t *env);

/* Core camera operation functions - STUB IMPLEMENTATIONS */
/* Users must implement these functions for their specific camera system */

/**
 * Start camera recording - STUB IMPLEMENTATION
 *
 * IMPLEMENTATION NOTE: This is a STUB function. Users must replace this with
 * camera-specific implementation. Examples:
 *
 * For OpenCamera integration via JNI:
 *   - Call JNI functions to start OpenCamera recording
 *   - Set clip name, quality, and duration parameters
 *   - Return AXIS2_SUCCESS if recording started successfully
 *
 * For V4L2 integration:
 *   - Open V4L2 device and configure capture parameters
 *   - Start video capture and encoding pipeline
 *   - Save to specified clip_name with given quality settings
 *
 * For IP camera integration:
 *   - Send RTSP or HTTP commands to start camera recording
 *   - Configure recording parameters on remote camera
 *   - Monitor recording status and handle errors
 *
 * @param env Apache Axis2/C environment
 * @param params Recording parameters (clip_name, quality, duration, format)
 * @return AXIS2_SUCCESS if recording started successfully, AXIS2_FAILURE otherwise
 */
AXIS2_EXTERN axis2_status_t AXIS2_CALL
camera_device_start_recording_impl(const axutil_env_t *env,
                                  const camera_recording_params_t *params);

/**
 * Stop camera recording - STUB IMPLEMENTATION
 *
 * IMPLEMENTATION NOTE: This is a STUB function. Users must replace this with
 * camera-specific implementation to stop active recording.
 *
 * @param env Apache Axis2/C environment
 * @return AXIS2_SUCCESS if recording stopped successfully, AXIS2_FAILURE otherwise
 */
AXIS2_EXTERN axis2_status_t AXIS2_CALL
camera_device_stop_recording_impl(const axutil_env_t *env);

/**
 * Get camera status - STUB IMPLEMENTATION
 *
 * IMPLEMENTATION NOTE: This is a STUB function. Users must replace this with
 * camera-specific implementation to query current camera state.
 *
 * @param env Apache Axis2/C environment
 * @return Camera status structure (caller must free), NULL on failure
 */
AXIS2_EXTERN camera_status_t* AXIS2_CALL
camera_device_get_status_impl(const axutil_env_t *env);

/**
 * Configure camera settings - STUB IMPLEMENTATION
 *
 * IMPLEMENTATION NOTE: This is a STUB function. Users must replace this with
 * camera-specific implementation to configure camera parameters.
 *
 * @param env Apache Axis2/C environment
 * @param settings Camera settings to apply
 * @return AXIS2_SUCCESS if settings applied successfully, AXIS2_FAILURE otherwise
 */
AXIS2_EXTERN axis2_status_t AXIS2_CALL
camera_device_configure_settings_impl(const axutil_env_t *env,
                                     const camera_settings_t *settings);

/**
 * Transfer file via SFTP with PKI authentication - STUB IMPLEMENTATION
 *
 * IMPLEMENTATION NOTE: This is a STUB function. Users must replace this with
 * SFTP client implementation using PKI (passwordless) authentication.
 *
 * Example integrations:
 *
 * For libssh2 integration:
 *   - Initialize libssh2 session and SFTP subsystem
 *   - Use libssh2_userauth_publickey_fromfile() for PKI authentication
 *   - Transfer file using libssh2_sftp_open() and libssh2_sftp_write()
 *
 * For system sftp command:
 *   - Use ssh-agent or ssh-add for key management
 *   - Execute: sftp -o PasswordAuthentication=no user@host
 *   - Transfer file with put command
 *
 * For paramiko (Python binding):
 *   - Create SSH client with paramiko.SSHClient()
 *   - Connect using private key authentication
 *   - Use paramiko.SFTPClient for file transfer
 *
 * @param env Apache Axis2/C environment
 * @param params SFTP transfer parameters (hostname, username, key path, etc.)
 * @return AXIS2_SUCCESS if file transferred successfully, AXIS2_FAILURE otherwise
 */
AXIS2_EXTERN axis2_status_t AXIS2_CALL
camera_device_sftp_transfer_impl(const axutil_env_t *env,
                                const sftp_transfer_params_t *params);

/**
 * Configure camera for multi-camera deployment - STUB IMPLEMENTATION
 *
 * IMPLEMENTATION NOTE: This is a STUB function for configuring a single camera
 * in a multi-camera setup. Each camera is STATELESS and independent.
 *
 * SMPTE Timecode Sync via Tentacle Sync:
 *   - Tentacle Sync devices are externally "jam synced" together
 *   - Each camera receives SMPTE timecode on audio channel 1 from its Tentacle Sync
 *   - Cameras record video + embedded audio timecode independently
 *   - Post-production uses timecode for frame-accurate sync
 *   - NO camera-to-camera communication required
 *
 * Multi-Camera Deployment Process:
 *   1. Deploy CameraControlService to each camera device (different IP addresses)
 *   2. Connect Tentacle Sync device to each camera's audio input
 *   3. "Jam sync" all Tentacle Sync devices together (external process)
 *   4. Use curl commands to start/stop recording on each camera individually
 *   5. SFTP transfer files to central storage after recording
 *   6. Post-production aligns using embedded SMPTE timecode
 *
 * Example curl commands for 3-camera setup:
 *   # A-Camera (Pixel 9 Pro at 192.168.1.10)
 *   curl -k --http2 -H "Content-Type: application/json" \
 *        -d '{"clip_name":"shoot_001_A-cam","quality":"4K","duration":1800}' \
 *        https://192.168.1.10:443/services/CameraControlService/startRecording
 *
 *   # B-Camera (Pixel 9a at 192.168.1.11)
 *   curl -k --http2 -H "Content-Type: application/json" \
 *        -d '{"clip_name":"shoot_001_B-cam","quality":"4K","duration":1800}' \
 *        https://192.168.1.11:443/services/CameraControlService/startRecording
 *
 *   # C-Camera (Pixel 9a at 192.168.1.12)
 *   curl -k --http2 -H "Content-Type: application/json" \
 *        -d '{"clip_name":"shoot_001_C-cam","quality":"4K","duration":1800}' \
 *        https://192.168.1.12:443/services/CameraControlService/startRecording
 *
 * @param env Apache Axis2/C environment
 * @param params Deployment-specific parameters for this camera
 * @return AXIS2_SUCCESS if camera configured successfully, AXIS2_FAILURE otherwise
 */
AXIS2_EXTERN axis2_status_t AXIS2_CALL
camera_device_configure_multicam_deploy_impl(const axutil_env_t *env,
                                             const multi_camera_deploy_params_t *params);

/**
 * File listing parameters structure
 */
typedef struct file_list_params
{
    axis2_char_t *pattern;           /**< Optional filter pattern (e.g., "*.mp4", "today") */
    axis2_char_t *directory;         /**< Directory to list (defaults to video storage) */
} file_list_params_t;

/**
 * File info structure for list results
 */
typedef struct file_info
{
    axis2_char_t *name;              /**< Filename */
    long size;                       /**< File size in bytes */
    axis2_char_t *modified;          /**< Last modified timestamp */
} file_info_t;

/**
 * File list result structure
 */
typedef struct file_list_result
{
    int file_count;                  /**< Number of files found */
    long total_size;                 /**< Total size of all files */
    file_info_t *files;              /**< Array of file info structures */
    axis2_char_t *directory;         /**< Directory that was listed */
} file_list_result_t;

/**
 * File deletion parameters structure
 */
typedef struct file_delete_params
{
    axis2_char_t *pattern;           /**< Pattern for files to delete */
} file_delete_params_t;

/* File list/delete parameter creation and free functions */
AXIS2_EXTERN file_list_params_t* AXIS2_CALL
file_list_params_create_from_json(const axutil_env_t *env,
                                  const axis2_char_t *json_string);

AXIS2_EXTERN void AXIS2_CALL
file_list_params_free(file_list_params_t *params,
                      const axutil_env_t *env);

AXIS2_EXTERN void AXIS2_CALL
file_list_result_free(file_list_result_t *result,
                      const axutil_env_t *env);

AXIS2_EXTERN file_delete_params_t* AXIS2_CALL
file_delete_params_create_from_json(const axutil_env_t *env,
                                    const axis2_char_t *json_string);

AXIS2_EXTERN void AXIS2_CALL
file_delete_params_free(file_delete_params_t *params,
                        const axutil_env_t *env);

/**
 * List video files on device - STUB IMPLEMENTATION
 *
 * IMPLEMENTATION NOTE: This is a STUB function. Users must replace this with
 * camera/storage-specific implementation to list video files.
 *
 * Example integrations:
 *
 * For Android integration:
 *   - Query MediaStore for video files in DCIM/OpenCamera directory
 *   - Use ContentResolver to get file metadata (size, date, duration)
 *   - Return JSON array of file info objects
 *
 * For Linux/V4L2 integration:
 *   - Use opendir/readdir to list files in video storage directory
 *   - Use stat() to get file size and modification time
 *   - Filter by pattern (glob matching)
 *
 * Pattern support (reference: Kanaha implementation):
 *   - Empty/NULL: List all video files
 *   - "*.mp4": List files matching wildcard
 *   - "today": List files from current date
 *   - "YYYY-MM-DD": List files from specific date
 *
 * @param env Apache Axis2/C environment
 * @param params File listing parameters (pattern, directory)
 * @return File list result structure (caller must free), NULL on failure
 */
AXIS2_EXTERN file_list_result_t* AXIS2_CALL
camera_device_list_files_impl(const axutil_env_t *env,
                              const file_list_params_t *params);

/**
 * Delete video files from device - STUB IMPLEMENTATION
 *
 * IMPLEMENTATION NOTE: This is a STUB function. Users must replace this with
 * camera/storage-specific implementation to delete video files.
 *
 * Example integrations:
 *
 * For Android integration:
 *   - Use MediaStore to delete files (requires WRITE_EXTERNAL_STORAGE permission)
 *   - Use ContentResolver.delete() for MediaStore entries
 *   - Also delete the actual file from filesystem
 *
 * For Linux integration:
 *   - Use unlink() to delete files matching pattern
 *   - Implement glob matching for wildcard patterns
 *
 * Pattern support (reference: Kanaha implementation):
 *   - Specific file: "VID_20260104_105049.mp4"
 *   - Wildcard: "*.mp4", "VID_2026*"
 *   - Date: "2026-01-04" (all files from that date)
 *   - Today: "today" (all files from current date)
 *   - All: "*" (delete all video files)
 *
 * SECURITY NOTE: Implementation should validate patterns to prevent:
 *   - Path traversal attacks (reject patterns containing "..")
 *   - Deletion outside video directory
 *
 * @param env Apache Axis2/C environment
 * @param params File deletion parameters (pattern)
 * @return AXIS2_SUCCESS if files deleted successfully, AXIS2_FAILURE otherwise
 */
AXIS2_EXTERN axis2_status_t AXIS2_CALL
camera_device_delete_files_impl(const axutil_env_t *env,
                                const file_delete_params_t *params);

/* Utility functions */
AXIS2_EXTERN axis2_char_t* AXIS2_CALL
camera_service_sanitize_input(const axutil_env_t *env,
                             const axis2_char_t *input,
                             int max_length);

#ifdef __cplusplus
}
#endif

#endif /* CAMERA_CONTROL_SERVICE_H */
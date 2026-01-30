/*
 *   Copyright 2004,2005 The Apache Software Foundation.
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 */

#ifndef AXIS2_NTLM_H
#define AXIS2_NTLM_H

/**
 * @file axis2_ntlm.h
 * @brief NTLM Authentication - REMOVED
 *
 * NTLM support has been completely removed from Axis2/C due to security vulnerabilities.
 * This header is retained only for API compatibility - attempting to use NTLM functions
 * will result in compilation or runtime errors.
 *
 * NTLM vulnerabilities include:
 * - Pass-the-hash attacks
 * - Relay attacks
 * - Weak cryptography (MD4/DES based)
 *
 * Microsoft is deprecating NTLM:
 * https://techcommunity.microsoft.com/blog/windows-itpro-blog/the-evolution-of-windows-authentication/4478637
 *
 * Recommended alternatives:
 * - Kerberos (for Windows domain environments)
 * - OAuth 2.0 (for modern authentication)
 * - mTLS (mutual TLS for certificate-based authentication)
 */

#error "NTLM authentication has been removed from Axis2/C. Use Kerberos, OAuth 2.0, or mTLS instead."

#endif /* AXIS2_NTLM_H */

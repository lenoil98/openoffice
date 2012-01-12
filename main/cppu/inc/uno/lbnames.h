/**************************************************************
 * 
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 * 
 *   http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 * 
 *************************************************************/


#ifndef _UNO_LBNAMES_H_
#define _UNO_LBNAMES_H_

#ifdef __cplusplus

#ifdef CPPU_ENV

#define CPPU_STRINGIFY_EX( x ) #x
#define CPPU_STRINGIFY( x ) CPPU_STRINGIFY_EX( x )

/** Name for C++ compiler/ platform, e.g. "gcc3", "msci" */
#define	CPPU_CURRENT_LANGUAGE_BINDING_NAME CPPU_STRINGIFY( CPPU_ENV )

#else

#error "No supported C++ compiler environment."
provoking error here, because PP ignores #error

#endif /* CPPU_ENV */

#endif /* __cplusplus */

/** Environment type name for binary C UNO. */
#define UNO_LB_UNO "uno"
/** Environment type name for ANSI C compilers. */
#define UNO_LB_C "c"
/** Environment type name for Java 1.3.1 compatible virtual machine. */
#define UNO_LB_JAVA "java"
/** Environment type name for CLI (Common Language Infrastructure). */
#define UNO_LB_CLI "cli"

#endif

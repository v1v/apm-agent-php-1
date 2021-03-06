/*
   +----------------------------------------------------------------------+
   | Elastic APM agent for PHP                                            |
   +----------------------------------------------------------------------+
   | Copyright (c) 2020 Elasticsearch B.V.                                |
   +----------------------------------------------------------------------+
   | Elasticsearch B.V. licenses this file under the Apache 2.0 License.  |
   | See the LICENSE file in the project root for more information.       |
   +----------------------------------------------------------------------+
 */

#include "mock_env_vars.h"
#include "mock_stdlib.h"
#include "basic_macros.h" // ELASTIC_APM_UNUSED
#include "unit_test_util.h"

static bool g_isMockEnvVarsInited = false;
static const StringToStringMap* g_mockEnvVarsMap = NULL;

void initMockEnvVars()
{
    ELASTIC_APM_CMOCKA_ASSERT( ! g_isMockEnvVarsInited );
    ELASTIC_APM_CMOCKA_ASSERT( g_mockEnvVarsMap == NULL );

    g_isMockEnvVarsInited = true;
}

void uninitMockEnvVars()
{
    ELASTIC_APM_CMOCKA_ASSERT( g_isMockEnvVarsInited );
    ELASTIC_APM_CMOCKA_ASSERT( g_mockEnvVarsMap == NULL );

    g_isMockEnvVarsInited = false;
}

void setMockEnvVars( const StringToStringMap* mockEnvVars )
{
    ELASTIC_APM_CMOCKA_ASSERT( g_isMockEnvVarsInited );
    ELASTIC_APM_CMOCKA_ASSERT_VALID_PTR( mockEnvVars );

    g_mockEnvVarsMap = mockEnvVars;
}

void resetMockEnvVars()
{
    ELASTIC_APM_CMOCKA_ASSERT( g_isMockEnvVarsInited );

    g_mockEnvVarsMap = NULL;
}

/**
 * mockGetEnv is used in "ConfigManager.c"
 * via ELASTIC_APM_GETENV_FUNC defined in unit tests' CMakeLists.txt
 *
 * mockGetEnv returns `char*` and not `const char*` on purpose
 * because real <stdlib.h> defines getenv as it's done below
 */
char* mockGetEnv( const char* name )
{
    ELASTIC_APM_CMOCKA_ASSERT( g_isMockEnvVarsInited );

    if ( g_mockEnvVarsMap == NULL ) return getenv( name );

    String value;
    const bool exists = getStringToStringMapEntry( g_mockEnvVarsMap, name, &value );
    return exists ? (char*)value : NULL;
}
ELASTIC_APM_SUPPRESS_UNUSED( mockGetEnv );

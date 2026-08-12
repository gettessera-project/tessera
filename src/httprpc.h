// Copyright (c) 2026 tessera core
// See COPYING for license.

#ifndef TESSERA_HTTPRPC_H
#define TESSERA_HTTPRPC_H

#include <any>

class JSONRPCRequest;
class UniValue;
enum HTTPStatusCode : int;

/** Start HTTP RPC subsystem.
 * Precondition; HTTP and RPC has been started.
 */
bool StartHTTPRPC(const std::any& context);
/** Interrupt HTTP RPC subsystem.
 */
void InterruptHTTPRPC();
/** Stop HTTP RPC subsystem.
 * Precondition; HTTP and RPC has been stopped.
 */
void StopHTTPRPC();

/** Execute a single HTTP request containing one or more JSONRPC requests.
 * Specified `jreq` will be modified and `status` will be returned.
 */
UniValue ExecuteHTTPRPC(const UniValue& valRequest, JSONRPCRequest& jreq, HTTPStatusCode& status);

/** Start HTTP REST subsystem.
 * Precondition; HTTP and RPC has been started.
 */
void StartREST(const std::any& context);
/** Interrupt RPC REST subsystem.
 */
void InterruptREST();
/** Stop HTTP REST subsystem.
 * Precondition; HTTP and RPC has been stopped.
 */
void StopREST();

#endif // TESSERA_HTTPRPC_H
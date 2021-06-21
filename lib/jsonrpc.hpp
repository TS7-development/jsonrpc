#pragma once

#include "asjson.hpp"
#include "callfromjson.hpp"
#include "error.hpp"
#include "errorcodes.hpp"
#include "fromjson.hpp"
#include "notification.hpp"
#include "procedure.hpp"
#include "request.hpp"
#include "response.hpp"
#include "util.hpp"

/** @mainpage TS7 JSON-RPC
 *
 * @section what What is it?
 * This is a boost based library to implement JSON based remote procedure calls (RPC). The two supported ways are
 * using a TCP server or client, but the library can be used with any form of underlying communication.
 *
 * @section why Why did I made it?
 * I have started working a lot with JSON-RPC and it felt fine. My first iteration was just writing functions that
 * take a JSON object as request and will provide q JSON object as result (response). This was working fine in the
 * beginning, but has lead to problems in later implementations. At any point I have started adding sequences to my
 * JSON-RPC server, which was meant as easy way to run a sequence of remote procedure calls in a row. This enabled
 * the client to do certain things without knowing the technical details of how to do it. Now when this sequence
 * handler wanted to call another procedure it had to create a JSON object and then it got a JSON object back as
 * result. This brings several problems. First off we lose the original function signature, which leads to not
 * getting informed by the compiler whenever it changes and the calling function did not adapted to it. Secondly
 * we waste runtime for encoding and decoding those requests and responses. And the last problem was, that it was
 * harder to debug. We alsways had to check the JSON object instead of having the arguments itself.
 *
 * In conclusion I was looking for a way to keep my original function signature, while still being able
 * to register / bind those to my JSON-RPC server or client. Which this library now is about.
 *
 * @section license Do I need to pay for the library?
 * No the library is a free to use open source library licensed under the Boost License.
 */


#include <iostream>
#include "spdlog/spdlog.h"
#include "PeerServer/PeerServer.h"
#include "SessionStorage/SessionStorage.h"

SessionStorage *sessionStorage = nullptr;

int main() {
//    spdlog::set_level(spdlog::level::debug);
//    auto server = Server(
//            8080,
//            8081);
//
//    server.start();
//    server.ResetKeycloak();
//    server.checkPermissionKeycloak("test.txt", "eyJhbGciOiJSUzI1NiIsInR5cCIgOiAiSldUIiwia2lkIiA6ICI5Y2dGRTdlODQ5ck1CMGZ4ZTJIRXVkLTNub1p6LWRCUEVwZGNaSE9MT3dZIn0.eyJleHAiOjE3MDM4NzI2ODksImlhdCI6MTcwMzg3MjM4OSwiYXV0aF90aW1lIjoxNzAzODcxOTk3LCJqdGkiOiI2Y2M5ZGVmNC03NTQ3LTQyMjUtOGJiMC1mNGQ2NmUyZWQ2NTIiLCJpc3MiOiJodHRwczovL2tleWNsb2FrLmF1dGguYXBvb3J2YTY0LmNvbS9yZWFsbXMvcHJvamV0LXNlY3UiLCJhdWQiOiJhY2NvdW50Iiwic3ViIjoiMmI4ZDgxNGEtYTU0YS00NTlmLTg3Y2UtMmVhNDRjMjg4MWY5IiwidHlwIjoiQmVhcmVyIiwiYXpwIjoiYWNjb3VudC1jb25zb2xlIiwibm9uY2UiOiIzMTA1MWY2Ny0wZWNjLTRjNWItODAxMS0wMTNlYzdiM2NkMWEiLCJzZXNzaW9uX3N0YXRlIjoiZjE0ZjRjNDYtNDI1ZC00NzVmLTkwM2MtMTY2ODI4MjNjYWYzIiwiYWNyIjoiMCIsInJlc291cmNlX2FjY2VzcyI6eyJhY2NvdW50Ijp7InJvbGVzIjpbIm1hbmFnZS1hY2NvdW50IiwibWFuYWdlLWFjY291bnQtbGlua3MiXX19LCJzY29wZSI6Im9wZW5pZCBwcm9maWxlIGVtYWlsIiwic2lkIjoiZjE0ZjRjNDYtNDI1ZC00NzVmLTkwM2MtMTY2ODI4MjNjYWYzIiwiZW1haWxfdmVyaWZpZWQiOmZhbHNlLCJwcmVmZXJyZWRfdXNlcm5hbWUiOiJ1c2VyMSJ9.RMNPBrilTFR2xcW7MFv2GPtHGLfFRdZmYyEw1Ue1Vl4UZECoXICCCeHojEL0BVKh-3Xk9qeejy9gtdfYXM92uh3WD0-QEDrpqjDnd55FS7CNwnjxlUnwpBplAbkBHvRSSgzaja4xVCGVTjcMNYcv4xTu_8Qy79Sybcp_7_aekzfP5rOM165W3o7WiK79o3Lt2f5Wx_xOeuo4KUj4PZ1iDm-lDcCWwRB8FplSv0VHg63s0gvpMWvlEEpr6xZNzFfXr4hpLulXGxlYg17qOVG-_lQmO3P38_7P_x3ggLrM1neSE7cv4wKb-Wtoz03p01Sk2AxOkYw4wUqsQ0zDHTiU0A", "download");
//    server.createKeycloakResource("test.txt", "user1");
    //server.run();
    sessionStorage = new SessionStorage(9000, 9007);
    auto peerServer = PeerServer(sessionStorage, 8080, 8081);
    peerServer.start();
    peerServer.run();
}
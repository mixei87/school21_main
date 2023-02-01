#include <fcgiapp.h>

int main() {
  FCGX_Init();
  FCGX_Request req;
  FCGX_InitRequest(&req, 0, 0);

  while (FCGX_Accept_r(&req) >= 0) {
    FCGX_FPrintF(req.out, "Content-type: text/html charset=utf-8\n\n");
    FCGX_FPrintF(req.out, "<!DOCTYPE html>\n");
    FCGX_FPrintF(req.out, "<html lang = \"en\">\n");
    FCGX_FPrintF(req.out, "<head>\n");
    FCGX_FPrintF(req.out, "<meta charset = \"UTF-8\">\n");
    FCGX_FPrintF(
        req.out,
        "<meta http - equiv = \"X-UA-Compatible\" content = \"IE=edge\">\n");
    FCGX_FPrintF(req.out,
                 "<meta name = \"viewport\" content =\"width=device-width,");
    FCGX_FPrintF(req.out, "initial-scale=1.0\">\n");
    FCGX_FPrintF(req.out, "<title>Super nginx</title>\n");
    FCGX_FPrintF(req.out, "</head>\n");
    FCGX_FPrintF(req.out, "<body>\n");
    FCGX_FPrintF(req.out, "<h1>Hello, world!</h1>\n");
    FCGX_FPrintF(req.out, "</body>\n");
    FCGX_FPrintF(req.out, "</html>\n");

    FCGX_Finish_r(&req);
  }
  return 0;
}

#include <epidemic_app.h>

#include <cinder/app/RendererGl.h>

using ci::app::RendererGl;
using epidemic::EpidemicApp;

void prepareSettings(EpidemicApp::Settings* settings) {
  settings->setResizable(false);
  settings->setTitle("Epidemic");
}

CINDER_APP(EpidemicApp, RendererGl, prepareSettings);

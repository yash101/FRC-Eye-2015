#ifndef BUILDCONF_H
#define BUILDCONF_H
#define CONFIGURATION_LOCATION     "app.cfg"//Where the default configuration file is located
#define LOG_LOCATION               "log.txt"//Where the default log it located
#define CAMERA_DEFAULT_ENABLE               //Enable the default camera. Disable this and the build may fail horribly!
#define CAMERA_DEFAULT_INITIALIZER      0   //Our camera will be initialized to use index (int) 0. you may also set it
                                            //to "location of remote stream or video file".
#define DEFAULT_PORT_INTERFACE          5800//Default port to access the web interface from
#define DEFAULT_PORT_CONNECTOR          5801//Default port for the client to connect to!
#endif // BUILDCONF_H

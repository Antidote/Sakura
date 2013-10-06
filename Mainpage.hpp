/*!
 *  \mainpage
 *
 *  \section welcome Welcome
 *  Welcome to the official Sakura Engine documentation. Here you will find a detailed
 *  view of all the Sakura Engine <a href="./annotated.html">classes</a> and functions. <br />
 *
 *  \section example Short example
 *  Here is a short example, to show you how to get Sakura started.
 *
 *  \code
 *  #include <iostream>
 *  #include <Engine.hpp>
 *
 *  #ifdef SFML_SYSTEM_LINUX
 *      #include <X11/Xlib.h>
 *  #endif
 *
 *  int main(int argc, char* argv[])
 *  {
 *     // If we are on a linux system we need to initialize the threads
 *  #ifdef SFML_SYSTEM_LINUX
 *     XInitThreads();
 *  #endif
 *      // We don't want console output slowing down execution.
 *      // This decouples the engine from the console, allowing you to output directly to stdout if you desire.
 *      std::ios_base::sync_with_stdio(false);
 *      Engine engine;
 *
 *  #ifndef DEBUG
 *      try
 *      {
 *  #endif
 *          engine.initialize(argc, argv);
 *          return engine.run();
 *  #ifndef DEBUG
 *      }
 *      catch(...)
 *      {
 *          // Sakura provides two ways to access the engine instance
 *          // First is by reference.
 *          sEngineRef().console().print(Console::Fatal, "An unhandled exception has occurred");
 *          // Second is by pointer.
 *          sEnginePtr()->setFullscreen(false);
 *      }
 *  #endif
 *      return 1;
 *  }
 *  \endcode
 *
 */

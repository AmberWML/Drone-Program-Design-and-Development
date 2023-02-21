/*! \mainpage Iteration 2 Index Page
 *
 * \section intro_sec Iteration 2
 *
 * A drone, a customer, and a package spawn in. The drone moves to the package,
 * picks it up, carries it to the customer, and drops it off.
 *
 * \image html iter2_UML.png "Figure 1: the Unified Modeling Language Diagram for Iteration 2" height=715 width=1000
 *
 * To build this project, navigate to its directory in the terminal, then run 'make', which uses the
 * associated Makefile. This creates the appropriate scripts in the folder bin. To run the project,
 * run bin/run.sh, then open your browser of choice and navigate to http://127.0.0.1:8081/. The
 * simulation will begin running. You may click and drag with the mouse to rotate the camera, and
 * you may center it on any of the Entities in the scene using the dropdown menu in the upper right.
* When you are done, do not forget to click "kill simulation" in the upper right.
 *
 * Functionality summary, edited for Iteration 2 from pet01631's Iteration 1 Design Document:\n
 * The project is centered around the class DroneSimulation, which inherits from DroneDeliverySystem,
 * a template for the simulation provided by ACME. It is built on a library called EntityProject, which
 * provides a skeleton for entities to be created and rendered in an online browser window.
 * DroneSimulation makes use of the classes Factory, DroneCoordinator, and SimpleUMNRouteManager to
 * manage the ensemble of Drones, Customers, and Packages. The Drones, Customers and Packages all
 * inherit from Entity; DroneSimulation creates them using JSON input files, a CSV of drone models,
 * and its Factory, then stores them in its list of Entities. JsonHelper assists throughout the
 * process by interpreting the JSON input files in Factory and in the constructors for Drones,
 * Customers, and Packages (see Figure 1).
 *
 * When the command is sent to schedule a delivery, DroneSimulation uses DroneCoordinator to assign
 * a free Drone to the Package and Customer. If there are no free drones, the Package is instead placed
 * in a Priority Queue of unassigned packages by priority. Whenever a drone in DroneSimulation's
 * Update() becomes free, DroneSimulation uses DroneCoordinator to assign them to the first package in the queue.
 * DroneCoordinator then uses its RouteManager to assign the Drone a route of SimpleUMNPoints to follow.
 * Meanwhile, DroneSimulation’s Update() method is repeatedly called, which calls the Update() method
 * on each Drone, Package, and Customer (see Figure 1). When the Drone receives the call, it points
 * itself toward the next point on its route and moves toward it, using its speed to limit its motion.
 * If it reaches its Package, it marks itself as carrying it, and from then on it maintains the Package’s
 * next to it. The Package, meanwhile, checks for its presence near the Customer that ordered it; if
 * it is within dropoff range, it marks itself as having been delivered. Whenever that occurs,
 * DroneSimulation’s Update() frees the Package’s memory and erases it from its vector list,
 * removing it from the simulation once delivered. Customer’s Update() method is called with every
 * DroneSimulation Update(), but it does not currently have any functionality--it is primed to become
 * more useful in future iterations, however.
 *
 * The last area of functionality is the Observer system. Both Packages and Drones extend EntityISubject,
 * which means that they can be observed by observers like WebSceneViewer. Packages and Drones both
 * now track a vector of observers. Whenever an event happens to them, they notify every observer
 * in their vector of observers of what has happened. This allows the WebSceneViewer to print out
 * text whenever a Package is scheduled, picked up, or delivered, for example.
 */

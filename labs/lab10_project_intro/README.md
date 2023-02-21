# Base Version of the Drone Delivery System Project for 3081W

This lab provides you with the base code (Initial Version) of the Drone Delivery System project for 3081W this semester. The objective of this lab is to get you oriented with the visual version of the Drone Delivery System that you will be working on quite a bit for the rest of the semester!

---

### Resources

It will be helpful to refer to these as you work on the project.

- Google Style Guide : https://google.github.io/styleguide/cppguide.html
- cpplint : https://github.com/google/styleguide/tree/gh-pages/cpplint

### Directory Structure

The directory structure for this lab is identical to that of the project.

- `project`:
   - directory that contains all the project subdirectories. The project subdirectories contain the source code, tests, executables, and documentation for the Drone Delivery System.

- `project/bin`: Contains shell scripts that make your life easier
  - `run.sh` runs the server when on Docker or Vole

  - `setup-server.sh` - Sets up your server on SSH or CSE Lab machines

  - `server.sh` - Runs remote web server on SSH or CSE Lab machines

  - More detailed instructions for running can be found here: https://github.umn.edu/umn-csci-3081-f20/shared-upstream/blob/support-code/README.md

- `project/docs`: Where all your documentation is typically stored, including your Doxyfile

- `project/src`:
  - Most of the source code (.cc AND .h files) for the Drone Delivery System is located here

- `project/tests` : directory where you will create and build your unit tests using google test.

- `project/web`: HTML / CSS / and JavaScript code used by your browser - When loaded by the browser with the web-server running, it enables you to configure and run the Drone Delivery System from your browser.

- `project/makefile`:
  - Used to create the simulation executable

  - Type `make` to build the system and its executable

  - Type `make test` to make executables that test certain elements within your system

  - Type `make clean` to remove all existing object, executable, and test files

---

## Getting Started

1. _pull_  new code into your support-code branch on your repo.

2. _merge_ the new lab code into your devel/development branch.

3. _cd_ (change directory) in  to the `project` directory.

4. Copy your Doxyfile from Lab 8 into the `project/docs` directory : `cp ../labs/lab08_style_doxy/docs/Doxyfile ./docs`

5. Copy your `mainpage.h` from Lab 8 into the `project/src` directory : `cp ../labs/lab08_style_doxy/src/mainpage.h ./src`

6. Edit your Doxyfile to include `/project/grades/Fall-2020/csci3081/dependencies/include/EntityProject` as an additional input

---

## Code Explanation

What we will be providing you with is the base code for the Drone Delivery System. The simulation will consist of entities like drones, customers, packages, etc. that you will have to design and implement according to the specifications given.

---

## The Lab Assignment

### Build the Drone Delivery System and run it

---

1. Open the **README.md** file in the top-level of your repo/shared-upstream

2. Follow the instructions to run the Drone Delivery System

3. Take a look at the system in a browser

4. Kill the server by clicking the kill button or `ctrl + C`
  - Please do kill the server, or it may take unexpected computer resources and you might get an email from CSE staff if you leave it running.

5. Make the tests via the Makefile and run them

6. Determine what needs to be added/changed based off of the output from the tests. It may be helpful to look at `project/tests/factory_test.cc` if you are unsure of what is making your tests fail
  - Something that may help is knowing that `details_` is a protected variable in the `entity` class. You may need to add a new method or overload an existing one to deal with it

7. As a result of successfully adding the correct code, you should be able to see a drone when viewing your server in browser

---

Push your code up to your repository for feedback (don't forget to add the new files and commit before pushing...).

---
# Note on feedback regeneration

## To trigger a feedback regeneration for this lab, you must modify a (any) file in `labs/lab10_project_intro`

The feedback server looks for changes in that folder and only regenerates if something within it has changed. Since the work for this lab will be done in `repo/project/`, it will not regenerate automatically when you change files in that directory.

If the feedback does not update after you push, it is either the same as before or the bot is down. Please check to be sure that you think the bot is down before notifying us.

I would recommend that you just make a small change to the this file (e.g. add a word) to trigger the feedback regeneration!


---
## Documentation Requirements
### For this lab (Optional):
  - Add detailed Doxygen comments to header files: https://www.doxygen.nl/manual/docblocks.html
    - We recommend that you use the Javadoc Style shown in the given link
    - Recommended commands: @brief, @param, @return
  - Write self-documenting C++ code according to the guidelines specified in chapters 11 and 32 in McConnell Code Complete.
  - Make sure your code is Google Style compliant using cpplint

### Project Documentation:
  - For project submissions, you are required to do all the documentation mentioned above to receive full credit on your submission.

### Congratulations, you have started the CSCI3081W project for Fall 2020!!!

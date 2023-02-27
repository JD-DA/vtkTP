

#include "exo-vtk-include.h"
#include "helpers.h"
#include "config.h"

// Globals
unsigned int counter = 0;

int main(int, char *[])
{

    vtkSmartPointer<vtkSphereSource> sphereSource =
        vtkSmartPointer<vtkSphereSource>::New();
    sphereSource->SetCenter(0.0, 0.0, 0.0);
    sphereSource->SetRadius(5.0);
    sphereSource->Update();

    vtkSmartPointer<vtkPolyDataMapper> mapper =
      vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(sphereSource->GetOutputPort());

    // Create an actor
    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);
    
  // Setup renderer, render window, and interactor
    vtkSmartPointer<vtkRenderer> renderer =vtkSmartPointer<vtkRenderer>::New();
    
    renderer->AddActor(actor);
    renderer->SetBackground(1,1,1);

  vtkSmartPointer<vtkRenderWindow> renderWindow =vtkSmartPointer<vtkRenderWindow>::New();
  renderWindow->AddRenderer(renderer);
  renderWindow->Render();

  // Start a timer 10 seconds to keep visible the rendering Window
  usleep(10*1000000);
	 
  return EXIT_SUCCESS;
}





#include "exo-vtk-include.h"
#include "helpers.h"
#include "config.h"
#include "vtkConeSource.h"
#include "vtkCamera.h"

// Globals
unsigned int counter = 0;

int main(int, char *[])
{
    vtkConeSource *cone = vtkConeSource::New();

    vtkSmartPointer<vtkConeSource> sphereSource =
        vtkSmartPointer<vtkConeSource>::New();
    cone->SetCenter(0.0, 0.0, 0.0);
    //cone->SetRadius(5.0);
    cone->SetHeight( 3.0 );
    cone->SetRadius( 0.5 );
    cone->SetResolution( 10 );
    cone->Update();



    vtkSmartPointer<vtkPolyDataMapper> mapper =
      vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(cone->GetOutputPort());

    // Create an actor
    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);

    // Create an actor
    vtkSmartPointer<vtkActor> actor2 = vtkSmartPointer<vtkActor>::New();
    actor2->SetMapper(mapper);
    actor2->SetPosition(1,0,0);
    actor->GetProperty()->SetColor(0.5,0.7,0.1);
    actor2->GetProperty()->SetColor(0.9,0.7,0.1);



    // Setup renderer, render window, and interactor
    vtkSmartPointer<vtkRenderer> renderer =vtkSmartPointer<vtkRenderer>::New();
    
    renderer->AddActor(actor);
    renderer->AddActor(actor2);
    renderer->SetBackground(1,1,1);

    renderer->GetActiveCamera()->SetPosition(5,10,10);
    renderer->GetActiveCamera()->SetFocalPoint(0,1,0);
    renderer->GetActiveCamera()->Azimuth(180);


  vtkSmartPointer<vtkRenderWindow> renderWindow =vtkSmartPointer<vtkRenderWindow>::New();
  renderWindow->AddRenderer(renderer);
  renderWindow->Render();
    vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();

    iren->SetRenderWindow(renderWindow);

    iren->Initialize();

    iren->Start();
  // Start a timer 10 seconds to keep visible the rendering Window
  usleep(10*1000000);
	 
  return EXIT_SUCCESS;
}



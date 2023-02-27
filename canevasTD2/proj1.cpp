

#include "exo-vtk-include.h"
#include "helpers.h"
#include "config.h"

#define FICHIER MY_MESHES_PATH "/noise.vtk"

int main(int, char *[])
{
    vtkCutter *cutter = vtkCutter::New();
    vtkPlane *plane = vtkPlane::New();
    
    vtkDataSetReader *reader = vtkDataSetReader::New();
       reader->SetFileName(FICHIER);
       
       vtkDataSetMapper *mapper = vtkDataSetMapper::New();
       mapper->SetInputConnection(reader->GetOutputPort());
    vtkLookupTable *lut = vtkLookupTable::New();




       vtkActor *actor = vtkActor::New();
       actor->SetMapper(mapper);
       
       vtkRenderer *ren = vtkRenderer::New();
       ren->AddActor(actor);
       
       vtkRenderWindow *renwin = vtkRenderWindow::New();
       renwin->SetSize(768, 768);
       renwin->AddRenderer(ren);

    double vals[4] = { 0.75, 0, 0.05, 1 };

    lut->SetTableValue(0, vals);

    mapper->SetLookupTable(lut);

    mapper->SetScalarRange(2.4,6);

    lut->Build();

       
       
       vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();
       iren->SetRenderWindow(renwin);
       renwin->Render();
       iren->Start();
  return EXIT_SUCCESS;
}



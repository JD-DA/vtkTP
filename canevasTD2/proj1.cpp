

#include "exo-vtk-include.h"
#include "helpers.h"
#include "config.h"

#define FICHIER MY_MESHES_PATH "/noise.vtk"

int main(int, char *[])
{
    
    vtkDataSetReader *reader = vtkDataSetReader::New();
       reader->SetFileName(FICHIER);
       
       vtkDataSetMapper *mapper = vtkDataSetMapper::New();
       mapper->SetInputConnection(reader->GetOutputPort());
    vtkLookupTable *lut = vtkLookupTable::New();

    mapper->SetLookupTable(lut);

    mapper->SetScalarRange(1,6);

    lut->Build();

    vtkContourFilter *cf = vtkContourFilter::New();
    cf->SetInputConnection(reader->GetOutputPort());
    cf->SetNumberOfContours(2);
    cf->SetValue(0, 2.4);
    cf->SetValue(1, 4);

    mapper->SetInputConnection(cf->GetOutputPort());
       
       vtkActor *actor = vtkActor::New();
       actor->SetMapper(mapper);
       
       vtkRenderer *ren = vtkRenderer::New();
       ren->AddActor(actor);
       
       vtkRenderWindow *renwin = vtkRenderWindow::New();
       renwin->SetSize(768, 768);
       renwin->AddRenderer(ren);



       
       
       vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();
       iren->SetRenderWindow(renwin);
       renwin->Render();
       iren->Start();
  return EXIT_SUCCESS;
}



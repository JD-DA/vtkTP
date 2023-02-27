

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

    vtkDataSetMapper *mapper2 = vtkDataSetMapper::New();
    vtkActor *actor2 = vtkActor::New();
    actor2->SetMapper(mapper2);
    mapper2->SetInputConnection(cutter->GetOutputPort());
    mapper2->SetScalarRange(1,6);
    mapper2->SetLookupTable(lut);



       vtkActor *actor = vtkActor::New();
       actor->SetMapper(mapper);

    vtkRenderer *ren = vtkRenderer::New();
    vtkRenderer *ren2 = vtkRenderer::New();
       ren->AddActor(actor);
         ren2->AddActor(actor2);
       
       vtkRenderWindow *renwin = vtkRenderWindow::New();
       renwin->SetSize(768, 768);
       renwin->AddRenderer(ren);
         renwin->AddRenderer(ren2);

    double vals[4] = { 0.75, 0, 0.05, 1 };

    lut->SetTableValue(0, vals);

    mapper->SetLookupTable(lut);

    mapper->SetScalarRange(2.4,6);

    lut->Build();

    for (int i = 0 ; i < 256 ; i++)
    {
        double vals[4] = { 1-(i/256.0), 0, (i/256.0), 1 };   //ou selon les valeurs de i,  A prend les valeurs de 1 à 0 et B de 0 à 1
        lut->SetTableValue(i, vals);
    }


    vtkContourFilter *cf = vtkContourFilter::New();
    cf->SetInputConnection(reader->GetOutputPort());
    cf->SetNumberOfContours(2);
    cf->SetValue(0, 2.4);
    cf->SetValue(1, 4);

    mapper->SetInputConnection(cf->GetOutputPort());


    plane->SetOrigin(0,0,0);
    plane->SetNormal(0,1,1);

    cutter->SetInputConnection(reader->GetOutputPort());
    //set implicte function
    cutter->SetCutFunction(plane);
    mapper2->SetInputConnection(cutter->GetOutputPort());

       
       vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();
       iren->SetRenderWindow(renwin);
       renwin->Render();

    ren->SetViewport(0.5, 0, 1, 1);

    ren2->SetViewport(0, 0, 0.5, 1);

       iren->Start();
  return EXIT_SUCCESS;
}



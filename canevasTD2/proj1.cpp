

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
       ren->AddActor(actor);
         ren->AddActor(actor2);
       
       vtkRenderWindow *renwin = vtkRenderWindow::New();
       renwin->SetSize(768, 768);
       renwin->AddRenderer(ren);

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


    int nbpas=2000;
    reader->Update();
    double bounds[6];
    vtkDataSet *polydata=reader->GetOutput();

    polydata->GetBounds(bounds);

    std::cout  << "xmin: " << bounds[0] << " "
               << "xmax: " << bounds[1] << std::endl
               << "ymin: " << bounds[2] << " "
               << "ymax: " << bounds[3] << std::endl
               << "zmin: " << bounds[4] << " "
               << "zmax: " << bounds[5] << std::endl;
    for (int i = 1 ; i < nbpas ; i++)
    {
        cf->SetValue(0, 2.4 + (i*0.001));
        renwin->Render();
        plane->SetOrigin(bounds[0]+i*bounds[1]/nbpas,bounds[2]+i*bounds[3]/nbpas,bounds[4]+i*bounds[5]/nbpas);
    }

  return EXIT_SUCCESS;
}



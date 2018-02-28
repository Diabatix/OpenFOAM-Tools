/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2011-2017 OpenFOAM Foundation
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

Application
    writeMeshQuality

Description
    This small tool writes the cell and face quality of the mesh. It does not
    account for dynamic meshes. It simply reads the mesh from the latest time
    step or constant folder. The mesh quality fields are written to the next
    time folder.

    In order to visualize faceNonOrtho and faceSkewness in paraview, first run
  
    foamToVTK -surfaceFields -fields '(faceNonOrtho faceSkewness)'

    and import the .vtk files.

    Author: Tech Queen Roxy (Diabatix nv, Belgium)

Usage
    \b writeMeshQuality [OPTION]

\*---------------------------------------------------------------------------*/

#include "fvCFD.H"
#include "volFields.H"
#include "cellQuality.H"

using namespace Foam;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
    #include "addRegionOption.H"
    #include "setRootCase.H"
    #include "createTime.H"
    #include "createNamedMesh.H"

    // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

    cellQuality cq(mesh);

    // Non-orthogonality
    volScalarField nonOrtho
    (
        IOobject
        (
            "cellNonOrtho",
            runTime.timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        mesh,
        dimensionedScalar("zero",dimless,0.0)
    );
    nonOrtho.primitiveFieldRef() = cq.nonOrthogonality();
    nonOrtho.write();
    
    // Skewness
    volScalarField skew
    (
        IOobject
        (
            "cellSkewness",
            runTime.timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        mesh,
        dimensionedScalar("zero",dimless,0.0)
    );
    skew.primitiveFieldRef() = cq.skewness();
    skew.write();

    // Face non-orthogonality
    surfaceScalarField fnonOrtho
    (
        IOobject
        (
            "faceNonOrtho",
            runTime.timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        mesh,
        dimensionedScalar("zero",dimless,0.0)
    );
    fnonOrtho.primitiveFieldRef() = cq.faceNonOrthogonality();
    fnonOrtho.write();

    // Face skewness
    surfaceScalarField fskewness
    (
        IOobject
        (
            "faceSkewness",
            runTime.timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        mesh,
        dimensionedScalar("zero",dimless,0.0)
    );
    fskewness.primitiveFieldRef() = cq.faceSkewness();
    fskewness.write();

    Info<< "End\n" << endl;

    return 0;
}


// ************************************************************************* //

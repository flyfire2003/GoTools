/*
 * Copyright (C) 1998, 2000-2007, 2010, 2011, 2012, 2013 SINTEF ICT,
 * Applied Mathematics, Norway.
 *
 * Contact information: E-mail: tor.dokken@sintef.no                      
 * SINTEF ICT, Department of Applied Mathematics,                         
 * P.O. Box 124 Blindern,                                                 
 * 0314 Oslo, Norway.                                                     
 *
 * This file is part of GoTools.
 *
 * GoTools is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version. 
 *
 * GoTools is distributed in the hope that it will be useful,        
 * but WITHOUT ANY WARRANTY; without even the implied warranty of         
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public
 * License along with GoTools. If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * In accordance with Section 7(b) of the GNU Affero General Public
 * License, a covered work must retain the producer line in every data
 * file that is created or manipulated using GoTools.
 *
 * Other Usage
 * You can be released from the requirements of the license by purchasing
 * a commercial license. Buying such a license is mandatory as soon as you
 * develop commercial activities involving the GoTools library without
 * disclosing the source code of your own applications.
 *
 * This file may be used in accordance with the terms contained in a
 * written agreement between you and SINTEF ICT. 
 */

#ifndef _GVGENERICTRIQUADPAINTABLE_H
#define _GVGENERICTRIQUADPAINTABLE_H


#include "GoTools/viewlib/gvPaintable.h"
#include "GoTools/viewlib/gvGenericTriMesh.h"
#include "GoTools/viewlib/gvGenericTriQuadMesh.h"
#ifdef _MSC_VER
#define NOMINMAX
#include <windows.h>
#endif
#include <GL/gl.h>

/** Documentation ...
    etc
 */

template <typename FloatType>
class gvGenericTriQuadPaintable : public gvPaintable
{
public:
    gvGenericTriQuadPaintable(gvGenericTriQuadMesh<FloatType>& mesh,
			  const gvColor& ncolor,
			  const gvColor& scolor,
			  int id)
	: gvPaintable(ncolor, scolor, id),
	  mesh_(mesh)
    {}
    gvGenericTriQuadPaintable(gvGenericTriQuadMesh<FloatType>& mesh,
			  const gvColor& ncolor,
			  int id)
	: gvPaintable(ncolor, id),
	  mesh_(mesh)
    {}
    virtual ~gvGenericTriQuadPaintable()
    {}

    virtual void paint()
    {
	GLenum fptype = GLfptype(FloatType());
	// Draw surfaces a little bit behind other things 
	glEnable (GL_POLYGON_OFFSET_FILL);
	glPolygonOffset (1.0, 1.0);

	// Deal with visibility and selection state
	if (!visible_) return;
	if (selected_) {
	    glMaterialfv(GL_FRONT_AND_BACK,
			 GL_AMBIENT_AND_DIFFUSE,
			 selected_color_.rgba);
	} else {
	    glMaterialfv(GL_FRONT_AND_BACK,
			 GL_AMBIENT_AND_DIFFUSE,
			 normal_color_.rgba);
	}
	GLfloat white[] = { 1.0, 1.0, 1.0, 1.0 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, white);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 100.0);

    // Set up the vertex and normal arrays
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glVertexPointer(3, fptype, 0, mesh_.vertexArray());
	//      std::cout << mesh_.vertexArray()[300] << ' '
	//  	      << mesh_.vertexArray()[301] << ' '
	//  	      << mesh_.vertexArray()[302] << std::endl;
	// @@@ We should check for the use of normals and textures in mesh_.
	glNormalPointer(fptype, 0, mesh_.normalArray());

    // Draw the triangles
	glDrawElements(GL_TRIANGLES, mesh_.numTriangles()*3,
		       GL_UNSIGNED_INT, mesh_.triangleIndexArray());
	glDrawElements(GL_QUADS, mesh_.numQuads()*3,
		       GL_UNSIGNED_INT, mesh_.quadIndexArray());

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisable(GL_POLYGON_OFFSET_FILL);
    }


protected:
    gvGenericTriQuadMesh<FloatType>& mesh_;
};





#endif // _GVGENERICTRIQUADPAINTABLE_H


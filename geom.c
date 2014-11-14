
/*! \fn int Angle (Vertex *A, Vertex *B, Vertex *C)
 *  \brief computes the "exterior angle" made by directed triple <\a A, \a B, \a C>.
 *  \param A first vertex in directed triple
 *  \param B second vertex in directed triple
 *  \param C last vertex in triple
 *
 *    The value return is LEFT, STRAIGHT or RIGHT when the "angle"
 *    is superior to, equal to or inferior to \f$\pi\f$. \par
 *    The returned value is equal to the sign of the determinant
 *    \f[ \left | \begin{array}{ccc} 1 & 1 & 1 \\ 
                                     x_A & x_B & x_c \\
                                     y_A & y_B & y_C \\
                  \end{array}
          \right |
      \f]
 *    if \f$(x_A, y_A), (x_B, y_B), (x_C, y_C)\f$ are the coordinates
 *    of vertices \f$A, B, C\f$. \par
 *    The value of the determinant is the signed area of the triangle
 *    \f$\Delta(A,B,C)\f$.
 */
int Angle (Vertex *A, Vertex *B, Vertex *C)
{ 
   Real det;

  if (A == B || B == C || A == C)
   return STRAIGHT;
  det = (C->y-B->y)*(B->x-A->x)-(B->y-A->y)*(C->x-B->x);
  if (det  < 0.0)
   return RIGHT;
  if (det == 0.0)
   return STRAIGHT;
  return LEFT;
}

/*! \fn  InCircle (Vertex *A, Vertex *B, Vertex *C, Vertex *Z)
 *  \param A the first vertex determining the circle
 *  \param B the second vertex determining the circle
 *  \param C the last vertex determining the circle
 *  \param Z the vertex to be tested against the circle \f$\Gamma(A,B,C)\f$.
 *  \brief determines wether vertex \a Z lies ouside, on, or inside
 *  the circle passing through \a A, \a B and \a C.
 *
 *  Let \f$\Gamma(ABC)\f$ be the circle around vertices \f$A, B, C\f$.
 *  Guibas and Stolfi have shown that the relative position of \f$Z\f$
 *  and \f$\Gamma(A,B,C)\f$ is equivalent to computing the sign 
 *  of determinant:
 *  \f[
        \left |
	  \begin{array}{cccc}
	   x_A     & y_A     & x_A^2+y_A^2  & 1 \\
	   x_B     & y_B     & x_B^2+y_B^2  & 1 \\
	   x_C     & y_C     & x_C^2+y_C^2  & 1 \\
	   x_Z     & y_Z     & x_Z^2+y_Z^2  & 1 \\
	  \end{array}
	\right |
 *  \f]
 *  corresponding to the relative position of vertex \f$Z\f$ and
 *  the plane through the projections of vertices \f$A,B,C\f$
 *  on the paraboloid of revolution with equation
 *  \f[ z = x^2+y^2. \f]
 *  According to Guibas & Stolfi's result, the current routine should return:
 *   -  -1 (OUT) if vertex \f$Z\f$ is outside circle \f$\Gamma(A,B,C)\f$,
 *   -  0 (ON)  if vertex \f$Z\f$ lies on circle \f$\Gamma(A,B,C)\f$, and
 *   -  1 (IN)  if vertex \f$Z\f$ lies inside circle \f$\Gamma(A,B,C)\f$.             
 *
 *  For convenience, as the only disturbing case is 
 *  "\f$D\f$ lies inside \f$\Gamma(A,B,C)\f$",
 *  the routine actually returns: 
 *     1 (IN)  if vertex \f$Z\f$ strictly lies inside circle \f$\Gamma(A,B,C)\f$,
 *     0       otherwise.
 *
 *  \warning Vertices \f$A,B,C\f$ are ASSUMED neither to be aligned or equal.
 *           Overflow might arise otherwise. User must check this condition
 *           with function Angle() before calling present function.
 */
int InCircle (Vertex *A, Vertex *B, Vertex *C, Vertex *Z)
{
   Real AZx = A->x - Z->x, AZy = A->y - Z->y,
        BZx = B->x - Z->x, BZy = B->y - Z->y,
        CZx = C->x - Z->x, CZy = C->y - Z->y,
          
        det = ((A->x + Z->x)*AZx + (A->y + Z->y)*AZy) * (BZx*CZy - BZy*CZx) +
              ((B->x + Z->x)*BZx + (B->y + Z->y)*BZy) * (CZx*AZy - CZy*AZx) +
              ((C->x + Z->x)*CZx + (C->y + Z->y)*CZy) * (AZx*BZy - AZy*BZx);
  
   
   /* should return (det > 0.0) ? IN : (det == 0.0) ? ON : OUT; really */
   return (det > 0.0) ? IN : 0;
}

/*
 * SmartPointer manages pointers. Automatically frees the pointer when it loses scope.
 * Copyright (C) 2002 Johnathan Ingram, jingram@rogueware.org
 *
 * This library is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU Lesser General Public
 *   License as published by the Free Software Foundation; either
 *   version 2.1 of the License, or (at your option) any later version.
 *
 *   This library is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *   Lesser General Public License for more details.
 *
 *   You should have received a copy of the GNU Lesser General Public
 *   License along with this library; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  US
 */

#ifndef __SMART_POINTER_H__
#define __SMART_POINTER_H__


using namespace std;

template<class T>
class SmartPointer
{

private:
   T*  pointer;

public:
   /* Constructors
    *
    */
   SmartPointer()
     : pointer(NULL) {};


   SmartPointer(
         T* pointer)
     : pointer(pointer) {};


   /* Destructor
    * Deletes the undelying pointer.
    *
    */
   ~SmartPointer()
   {
      // Destroy the current pointer if it is assigned.
      destroy();
   };
      

   /* Overload the assignment operator.
    * Assigns the underlying pointer deleting the existing one if necessary.
    *
    */
   void 
   operator=(
         T* p)
   {
      // Destroy the current pointer if it is assigned.
      destroy();
      pointer = p;   
   };
   

   /* Returns the underlying pointer.
    *
    */
   T* 
   operator->()
   {
      return pointer;
   };


   /* Returns a reference to the object the underlying pointer points to.
    *
    */
   T& 
   operator*()
   {
      return *pointer;
   };


   /* Deletes the underlying pointer.
    *
    */
   void
   destroy()
   {
      if (pointer)
      {
         delete pointer;
         pointer = NULL;
      }
   };
   

   /* Release the pointer.
    * The object is no longer responsible for the pointer.
    *
    * @return        Returns the pointer
    *
    */
   T*
   release()
   {
      T* tmp = pointer;
      pointer = NULL;
      
      return tmp;
   };


   /* Get a copy of the reference.
    *
    * @return        Returns the pointer
    *
    */
   T*
   get()
   {
      return pointer;      
   }
};



#endif



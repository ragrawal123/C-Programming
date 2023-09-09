#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>

#include "object.h"
#include "graphics.h"

/*
 * Function to read a file and allocate an entire object, filling it with
 * information from the file and allocating different things that the object is
 * made up of in order to have a full and complete object.
 */

object *read_object(const char *filename) {
  FILE *fp = fopen(filename, "r");
  if (!fp) {
    fp = NULL;
    return NULL;
  }

  int success = 0;
  char name[100] = "";
  success = fscanf(fp, "%s\n", name);
  if (success != 1) {
    fclose(fp);
    fp = NULL;
    return NULL;
  }

  object *new_object = malloc(sizeof(object)); //malloc
  if (!new_object) {
    fclose(fp);
    fp = NULL;
    return NULL;
  }

  success = fscanf(fp, "%d %d\n", &(new_object->num_points),
                   &(new_object->num_polygons));
  if (success != 2) {
    fclose(fp);
    fp = NULL;
    return NULL;
  }

  int num = 0;
  new_object->points = malloc(new_object->num_points * sizeof(point)); //malloc
  if (!new_object->points) {
    fclose(fp);
    fp = NULL;
    return NULL;
  }

  int is_regular = 1;
  for (int i = 0; i < new_object->num_points; i++) {
    point *p = &(new_object->points[i]);
    success = fscanf(fp, "%d %lf %lf %lf\n", &num, &(p->arr[0]),
                     &(p->arr[1]), &(p->arr[2]));
    if (num == 0) {
      is_regular = 0;
    }
    if (success != 4) {
      fclose(fp);
      fp = NULL;
      return NULL;
    }
  }

  new_object->polygons = malloc(new_object->num_polygons * sizeof(polygon));
  if (!new_object->polygons) {
    fclose(fp);
    fp = NULL;
    return NULL;
  }

  char color[100] = "";
  for (int i = 0; i < new_object->num_polygons; i++) {
    polygon *poly = &(new_object->polygons[i]);

    success = fscanf(fp, "%s %s %d ", name, color, &poly->num_points);
    if (success != 3) {
      fclose(fp);
      fp = NULL;
      return NULL;
    }
    name_to_rgb(color, &poly->r, &poly->g, &poly->b);

    poly->point_arr = malloc(poly->num_points * sizeof(point *)); //malloc
    for (int j = 0; j < poly->num_points; j++) {
      int point_index = 0;
      success = fscanf(fp, "%d", &point_index);
      if (success != 1) {
        fclose(fp);
        fp = NULL;
        return NULL;
      }

      if (is_regular) {
        poly->point_arr[j] = &new_object->points[point_index - 1];
      }
      else {
        poly->point_arr[j] = &new_object->points[point_index];
      }
    }
  }

  fclose(fp);
  fp = NULL;
  return new_object;

} /* read_object() */

/*
 * Function to deallocate a singular object and all the content within it.
 */

void free_object(object *obj) {
  if (obj == NULL) {
    return;
  }

  for (int i = 0; i < obj->num_polygons; i++) {
    polygon *poly = &(obj->polygons[i]);
    free(poly->point_arr);
    poly->point_arr = NULL;
  }

  free(obj->polygons);
  obj->polygons = NULL;
  free(obj->points);
  obj->points = NULL;
  free(obj);
  obj = NULL;
} /* free_object() */
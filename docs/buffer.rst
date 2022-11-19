.. highlight:: c

Buffers
========

The ``buffer_t`` handle provides an interface to a linear or ring buffer 
object.  The buffer type is passed as an argument during initialization
and the buffer i/o functions will behave accordingly.


Ring Buffer
-------------

::

  buffer_t *buf = NULL;
  buffer_init(&buf, 0x1000, RING);

Linear Buffer
--------------

::

  buffer_t *buf = NULL;
  buffer_init(&buf, 0x1000, LINEAR);

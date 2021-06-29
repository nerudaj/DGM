# dgm::Buffer

This class is a special version of vector. It is supposed to have predefined maximum capacity (which can be changed, even thought it is discouraged) and it has O(1) complexity of **both** adding and removing of an object.

It does not retain the order of items in the array, namely each removal invalidates all iterators and indexes to objects. However, it is internally implemented as a array of pointers and thus any reference to object in array will remain unaffected, unless resize method is called.

This data structure is supposed to be used whenever order of objects is not important, but objects are created and destroyed fairly often and there is known upper limit to their count (usually projectiles, particles and such).

The dgm::Buffer has two properties that shall not be confused - capacity and size. Capacity is the maximum number of objects that user can add to buffer at any given time. Size is the number of objects that were added to buffer.

It is important to notice, that all iterators will only iterate over added objects (that means indices from 0 to size), but user can iterate over all objects in the buffer (indices from 0 to capacity). User should iterate over all objects only once - during initialization. After that, user should only iterate over currently added objects.

When the object is removed, the memory location is not freed and object retains all of its properties. When expand is called, the object is accessible for iterating and should be reset to somewhat consistent state (resources should still remain allocated, but properties like position, damage, velocity should reset).

## Expected workflow

1. First, user has to resize the buffer to the desired max capacity.
2. User should iterate over all objects in the buffer and initialize them, but not use them (ie.: bind a texture to a particle, but not spawn it anywhere on screen)
3. User can call expand/remove methods and iterate over objects that are currently added and use them (spawn, update, render).

## Note about removing

Following snipped demonstrates expected workflow when removing items:

```
for (unsigned i = 0; i < buf.size(); i++) {
	buf[i].doLogic();
	if (buf[i].shouldBeRemoved()) {
		buf.remove(i--);
	}
}
```

Note the decrement in the remove call. When removing object from index `I`, that object will be swapped with the last valid object in the Buffer (and thus order is broken and iterators invalidated) and then the size of the buffer is decremented by one. That means the index `I` now holds completely different, but valid object and we need to iterate over that index once again.

Also note that swapping is done on a pointer level and thus you don't need to implement custom swap method for all your objects.
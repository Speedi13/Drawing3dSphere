# Drawing 3D Sphere
Drawing a 3D Sphere in C++
<br><br>
![Demo pic](https://i.imgur.com/pE5FU9b.jpg)
<br>
## How it works
First I draw horizontal circles, then I connect them with lines.<br>
Red are the circles, blue are the connection lines:<br>
![picture](https://i.imgur.com/zRo8gfa.png)


## Function usage
```cpp
int iSides = pExplosionEntityData->m_BlastRadius * 8;
if ( Settings->m_bGrenadeSphere )
    DrawSphere( position, transform, pExplosionEntityData->m_BlastRadius, iSides, dwColor );
```


## Demo Video
Watch on Youtube:<br>
https://www.youtube.com/watch?v=tOeF1u-Y2jk

Or as direct video in a bit better quality:<br>
https://giant.gfycat.com/SlimForcefulBrontosaurus.webm

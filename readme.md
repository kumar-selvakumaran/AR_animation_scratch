
# AR application with animated graphic objects from scratch

- This project uses the a checkerboard as a marker to anchor virtual objects and interactions are done after camera calibrations and mapping the object to the appropriate world coordinates.

The following graph primitives are implemented:
- a root 2n sided regular polygon 
- rotation tranformations to duplicate the polygons at different angle creating an approximate spher as n, and the number duplicates increase



 <table>
  <tr>
    <th>
      <video width="320" height="240" controls autoplay>
        <source src="./videos/2_4_120.mp4" type="video/mp4">
      </video>
    </th> 
    <th>
      <video width="320" height="240" controls autoplay>
        <source src="videos/6_8_120.mp4" type="video/mp4">
      </video>
    </th>
    <th>  
      <video width="320" height="240" controls autoplay>
        <source src="videos/16_20_240.mp4" type="video/mp4">
      </video>
    </th>
  </tr>
</table> 

- Time as a function of camera fps
- Angular velocity along each axis by applying rotation transformations at appropriate angles each frame
- parabolic trajectory, and acceleration modelling gravity.

## Key techniques:

- Histogram of Oriented gradients
- ResNet18 embeddings
- Strided histogram matching
- Laws Filters
- Morpholigcal operations (opening, closing, dilation, erosion)
- Distance metrics : L2 Norm, and Histogram Intersection.

## setup:

### key features:

- Windows 11, WSL, Docker, Flask, X11 screen

### requirements: 
- docker desktop version : v4.22.1
- docker version : 24.0.5
- (WSL) Ubuntu-22.04
<video width="320" height="240" controls>
  <source src="submission/VIDEOS/6_8_120.mp4" type="video/mp4">
</video>

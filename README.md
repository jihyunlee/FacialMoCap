# Facial Motion Capture
`Facial MoCap` iOS app enables to animate a character in real-time using blend shape coefficients recognized by ARKit. 

<b>IMAGE; iPhone screenshot</b>

### ARKit's Blendshape Location
For each key, the corresponding value is a floating point number indicating the current position of that feature relative to its neutral configuration, ranging from 0.0 (neutral) to 1.0 (maximum movement)

#### Eyes
<img src="images/eyeBlinkLeft.png" width="360" />&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;<img src="images/eyeBlinkRight.png" width="360" />
<img src="images/eyeLookDownLeft.png" width="360" />&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;<img src="images/eyeLookDownRight.png" width="360" />
<img src="images/eyeLookInLeft.png" width="360" />&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;<img src="images/eyeLookInRight.png" width="360" />
<img src="images/eyeLookOutLeft.png" width="360" />&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;<img src="images/eyeLookOutRight.png" width="360" />
<img src="images/eyeLookUpLeft.png" width="360" />&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;<img src="images/eyeLookUpRight.png" width="360" />
<img src="images/eyeSquintLeft.png" width="360" />&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;<img src="images/eyeSquintRight.png" width="360" />
<img src="images/eyeWideLeft.png" width="360" />&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;<img src="images/eyeWideRight.png" width="360" />

##### Jaw
<img src="images/jawOpen.png" width="300" />



### iOS app installation
1. Download Xcode from App Store
2. Open FacialMotionCapture.xcodeproj with Xcode
3. 

### Set up OSC environment


### Examples
There are [examples](/examples) in Cinder and Unreal Engine. As long as your application supports [OSC(Open Sound Control)](https://en.wikipedia.org/wiki/Open_Sound_Control), it can receive blendshape data from the `Facial MoCap` app.

### Reference
- [ARKit's ARFaceAnchor BlendShapeLocation](https://developer.apple.com/documentation/arkit/arfaceanchor/blendshapelocation)

//
//  AppDelegate.swift
//  FacialMotionCapture
//
//  Created by Jihyun Lee on 3/31/20.
//  Copyright © 2020 Jihyun Lee. All rights reserved.
//

import Foundation
import UIKit
import SceneKit
import ARKit

class ViewController: UIViewController, ARSCNViewDelegate, ARSessionDelegate {
    
    // MARK: Outlets

    @IBOutlet var sceneView: ARSCNView!

    // MARK: Properties
    
    var contentNode: SCNNode?
    var currentFaceAnchor: ARFaceAnchor?

    
    // MARK: - View Controller Life Cycle

    override func viewDidLoad() {
        super.viewDidLoad()

        sceneView.delegate = self
        sceneView.session.delegate = self
        sceneView.automaticallyUpdatesLighting = true
        
        guard let url = Bundle.main.url(forResource: "model", withExtension: "scn", subdirectory: "Models.scnassets") else { fatalError("Model resource not in bundle!") }
        let modelNode = SCNReferenceNode(url: url)!
        modelNode.load()
        contentNode = modelNode
    }

    override func viewDidAppear(_ animated: Bool) {
        super.viewDidAppear(animated)
        
        // AR experiences typically involve moving the device without
        // touch input for some time, so prevent auto screen dimming.
        UIApplication.shared.isIdleTimerDisabled = true
        
        // "Reset" to run the AR session for the first time.
        resetTracking()
    }
    
    
    // MARK: - ARSessionDelegate

    func session(_ session: ARSession, didFailWithError error: Error) {
        guard error is ARError else { return }
        
        let errorWithInfo = error as NSError
        let messages = [
            errorWithInfo.localizedDescription,
            errorWithInfo.localizedFailureReason,
            errorWithInfo.localizedRecoverySuggestion
        ]
        let errorMessage = messages.compactMap({ $0 }).joined(separator: "\n")
        
        DispatchQueue.main.async {
            self.displayErrorMessage(title: "The AR session failed.", message: errorMessage)
        }
    }
    
    func resetTracking() {
        guard ARFaceTrackingConfiguration.isSupported else { return }
        let configuration = ARFaceTrackingConfiguration()
        configuration.isLightEstimationEnabled = true
        sceneView.session.run(configuration, options: [.resetTracking, .removeExistingAnchors])
    }
    
    
    // MARK: - ARSCNViewDelegate
    
    func renderer(_ renderer: SCNSceneRenderer, didAdd node: SCNNode, for anchor: ARAnchor) {
        guard let faceAnchor = anchor as? ARFaceAnchor else { return }
        currentFaceAnchor = faceAnchor
        
        // If this is the first time with this anchor, get the controller to create content.
        // Otherwise (switching content), will change content when setting `selectedVirtualContent`.
        if node.childNodes.isEmpty, (contentNode != nil) {
            node.addChildNode(contentNode!)
        }
    }

    func renderer(_ renderer: SCNSceneRenderer, didUpdate node: SCNNode, for anchor: ARAnchor) {
        guard let faceAnchor = anchor as? ARFaceAnchor
            else { return }
   
        let blendShapes = faceAnchor.blendShapes

        DispatchQueue.main.async {
            
            // This will only work correctly if the shape keys are given the exact same name as the blendshape names
            print("-----------------------------------------")
            for (key, value) in blendShapes {
                if let fValue = value as? Float {
                    print(key.rawValue, ": ", CGFloat(fValue))
                    self.contentNode?.childNodes[0].morpher?.setWeight(CGFloat(fValue), forTargetNamed: key.rawValue)
                }
            }
        }
        
        // Reference:
        // ARFaceAnchor.BlendShapeLocation
        // https://developer.apple.com/documentation/arkit/arfaceanchor/blendshapelocation
        
        // Eye:
        // eyeBlinkLeft, eyeLookDownLeft, eyeLookInLeft, eyeLookOutLeft, eyeLookUpLeft, eyeSquintLeft, eyeWideLeft
        // eyeBlinkRight, eyeLookDownRight, eyeLookInRight, eyeLookOutRight, eyeLookUpRight, eyeSquintRight, eyeWideRight
        
        // Jaw:
        // jawForward, jawLeft, jawRight, jawOpen
        
        // Mouth:
        // mouthClose, mouthFunnel, mouthPucker, mouthLeft, mouthRight, mouthSmileLeft, mouthSmileRight,
        // mouthFrownLeft, mouthFrownRight, mouthDimpleLeft, mouthDimpleRight, mouthStretchLeft, mouthStretchRight
        // mouthRollLower, mouthRollUpper, mouthShrugLower, mouthShrugUpper, mouthPressLeft, mouthPressRight
        // mouthLowerDownLeft, mouthLowerDownRight, mouthUpperUpLeft, mouthUpperUpRight

        // Eyebrows:
        // browDownLeft, browDownRight, browInnerUp, browOuterUpLeft, browOuterUpRight
        
        // Cheeks:
        // cheekPuff, cheekSquintLeft, cheekSquintRight
        
        // Nose:
        // noseSneerLeft, noseSneerRight
        
        // Tongue:
        // tongueOut
        
        
        // example
//        if let eyeBlinkRight = blendShapes[.eyeBlinkRight] as? Float {
//            print("eyeBlinkRight: ", eyeBlinkRight)
//        }
    }
    
    
    // MARK: - Error handling
    
    func displayErrorMessage(title: String, message: String) {
        // Present an alert informing about the error that has occurred.
        let alertController = UIAlertController(title: title, message: message, preferredStyle: .alert)
        let restartAction = UIAlertAction(title: "Restart Session", style: .default) { _ in
            alertController.dismiss(animated: true, completion: nil)
            self.resetTracking()
        }
        alertController.addAction(restartAction)
        present(alertController, animated: true, completion: nil)
    }
    
}

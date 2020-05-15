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

let UDP_IPADDRESSES = ["10.63.111.129", "10.63.111.130"]
let UDP_PORT = 1204

class ViewController: UIViewController, UIGestureRecognizerDelegate, ARSCNViewDelegate, ARSessionDelegate {
    
    // MARK: Outlets

    @IBOutlet var sceneView: ARSCNView!
    @IBOutlet weak var iconsView: UIView!
    @IBOutlet weak var slidersView: SliderView!
    @IBOutlet weak var sliderIcon: UIImageView!

    // MARK: Properties
    
    var contentNode: SCNNode?
    var currentFaceAnchor: ARFaceAnchor?
    
    var blendShapesArray: [String] = []

    // Network
    var socket = UDPBroadcaster(port: UDP_PORT)
    
    var showIPAddressView = false
    var showSliderView = false
    var showMesh = false
    var isRecording = false

    // MARK: - View Controller Life Cycle

    override func viewDidLoad() {
        super.viewDidLoad()

        sceneView.delegate = self
        sceneView.session.delegate = self
        sceneView.automaticallyUpdatesLighting = true
        sceneView.showsStatistics = false
        
        // Icon bar
        self.sliderIcon.addGestureRecognizer(UITapGestureRecognizer(target: self, action: Selector(("onSliders:"))))
        self.sliderIcon.isUserInteractionEnabled = true
        
        // Load model
        guard let url = Bundle.main.url(forResource: "model", withExtension: "scn", subdirectory: "Models.scnassets") else { fatalError("Model resource not in bundle!") }
        let modelNode = SCNReferenceNode(url: url)!
        modelNode.load()
        contentNode = modelNode
        
        // Init a list of IP addresses to broadcast
        socket.setIpAddresses(array: UDP_IPADDRESSES)
        
        slidersView._init()
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
   
        var jsonObject: [String : Float] = [:]

        DispatchQueue.main.async {
            
            for (key, value) in faceAnchor.blendShapes {
                if var fValue = value as? Float {

                    // clamp the value
                    fValue = fValue.clamped(to: 0.0...1.0)
                    
                    // remap
                    if let range = self.slidersView.ranges.first(where: {$0.name == key.rawValue}) {
                        fValue = fValue * Float(range.max - range.min) + Float(range.min);
                    }

                    jsonObject[key.rawValue] = round(10000 * fValue)/10000
                    self.contentNode?.childNodes[0].morpher?.setWeight(CGFloat(fValue), forTargetNamed: key.rawValue)
                }
            }
            
            var jsonString = "{"
            jsonString.append(jsonObject.map{ "\"\($0)\":\($1)" }.joined(separator: ","))
            jsonString.append("}")
//            print(jsonString)
                
            if(self.isRecording) {
                self.blendShapesArray.append(jsonString)
            }
            
            _ = self.socket.sendToList(jsonString)
        }
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
    
    // MARK: - Record
    
    @IBAction func onRecord(_ sender: UIButton) {
        
        if(!self.isRecording) {
            if let image = UIImage(named: "StopImage") {
                sender.setImage(image, for: .normal)
            }
            
            self.blendShapesArray = []
            self.isRecording = true
        }
        else {
            if let image = UIImage(named: "RecordImage") {
                sender.setImage(image, for: .normal)
            }
            
            self.isRecording = false
            _FileManager.writeFile(array: self.blendShapesArray)
        }
    }
    
    // MARK: - Blendshape Location Sliders
    
    @IBAction func onSliders(_ sender: UIImageView) {
        
        self.slidersView.isHidden = self.showSliderView
        
        if self.showSliderView {
            sliderIcon.image = UIImage(named: "SliderIcon")
            
            ViewController.viewSlideInFromTopToBottom(view: self.slidersView)
        }
        else {
            sliderIcon.image = UIImage(named: "SliderFillIcon")
            ViewController.viewSlideInFromBottomToTop(view: self.slidersView)
        }

        self.showSliderView = !self.showSliderView
    }
    
}

extension Comparable {
    func clamped(to limits: ClosedRange<Self>) -> Self {
        return min(max(self, limits.lowerBound), limits.upperBound)
    }
}

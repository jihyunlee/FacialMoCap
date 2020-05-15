//
//  SliderView.swift
//  FacialMotionCapture
//
//  Created by Jihyun Lee on 5/9/20.
//  Copyright © 2020 Jihyun Lee. All rights reserved.
//

import Foundation
import UIKit

// Reference:
// ARFaceAnchor.BlendShapeLocation
// https://developer.apple.com/documentation/arkit/arfaceanchor/blendshapelocation

let BLENDSHAPE_LOCATIONS = [
    "eyeBlink_L", "eyeBlink_R",
    "eyeLookDown_L", "eyeLookDown_R",
    "eyeLookIn_L", "eyeLookIn_R",
    "eyeLookOut_L", "eyeLookOut_R",
    "eyeLookUp_L", "eyeLookUp_R",
    "eyeSquint_L", "eyeSquint_R",
    "eyeWide_L", "eyeWide_R",
    
    "jawLeft", "jawRight",
    "jawForward", "jawOpen",
    
    "mouthClose", "mouthFunnel", "mouthPucker",
    "mouthLeft", "mouthRight",
    "mouthSmile_L", "mouthSmile_R",
    "mouthFrown_L", "mouthFrown_R",
    "mouthDimple_L", "mouthDimple_R",
    "mouthStretch_L", "mouthStretch_R",
    "mouthRollLower", "mouthRollUpper",
    "mouthShrugLower", "mouthShrugUpper",
    "mouthPress_L", "mouthPress_R",
    "mouthLowerDown_L", "mouthLowerDown_R",
    "mouthUpperUp_L", "mouthUpperUp_R",
    
    "browDown_L", "browDown_R",
    "browInnerUp", "browOuterUp_L", "browOuterUp_R",
    
    "cheekPuff", "cheekSquint_L", "cheekSquint_R",
    
    "noseSneer_L", "noseSneer_R",
    
    "tongueOut"
]

class Range {

    var name: String = ""
    var min: CGFloat = 0.0
    var max: CGFloat = 1.0

    init(name: String) {
        self.name = name
        self.min = 0.0
        self.max = 1.0
    }

    func set(min: CGFloat, max: CGFloat) {
        self.min = min
        self.max = max
    }
}
    
public class SliderView: UIView, RangeSeekSliderDelegate {
    
    // MARK: Outlets
    
    @IBOutlet weak var rangeSlider: RangeSeekSlider!
    @IBOutlet weak var eyeLabel: UILabel!
    @IBOutlet weak var jawLabel: UILabel!
    @IBOutlet weak var mouthLabel: UILabel!
    @IBOutlet weak var eyebrowLabel: UILabel!
    @IBOutlet weak var cheekLabel: UILabel!
    @IBOutlet weak var noseLabel: UILabel!
    @IBOutlet weak var tongueLabel: UILabel!
    @IBOutlet weak var currentLabel: UILabel!
    @IBOutlet weak var prevImage: UIImageView!
    @IBOutlet weak var nextImage: UIImageView!


    // MARK: Properties
    
    let eyeIndex: Int = 0
    let jawIndex: Int = 14
    let mouthIndex: Int = 18
    let eyebrowIndex: Int = 41
    let cheekIndex: Int = 46
    let noseIndex: Int = 49
    let tongueIndex: Int = 51
    var index: Int = 0
    
    var ranges: [Range] = []

    override init(frame: CGRect) {
        super.init(frame: frame)
    }

    convenience init() {
        self.init(frame: CGRect.zero)
    }

    required init?(coder acoder: NSCoder) {
        super.init(coder: acoder)
    }
    
    func _init()
    {
        // Location tab bar
        self.eyeLabel.layer.masksToBounds = true
        self.eyeLabel.layer.cornerRadius = 5
        self.eyeLabel.isUserInteractionEnabled = true
        self.eyeLabel.addGestureRecognizer(UITapGestureRecognizer(target: self, action: Selector(("onTab:"))))

        self.jawLabel.layer.masksToBounds = true
        self.jawLabel.layer.cornerRadius = 5
        self.jawLabel.isUserInteractionEnabled = true
        self.jawLabel.addGestureRecognizer(UITapGestureRecognizer(target: self, action: Selector(("onTab:"))))
        
        self.mouthLabel.layer.masksToBounds = true
        self.mouthLabel.layer.cornerRadius = 5
        self.mouthLabel.isUserInteractionEnabled = true
        self.mouthLabel.addGestureRecognizer(UITapGestureRecognizer(target: self, action: Selector(("onTab:"))))
        
        self.eyebrowLabel.layer.masksToBounds = true
        self.eyebrowLabel.layer.cornerRadius = 5
        self.eyebrowLabel.isUserInteractionEnabled = true
        self.eyebrowLabel.addGestureRecognizer(UITapGestureRecognizer(target: self, action: Selector(("onTab:"))))
        
        self.cheekLabel.layer.masksToBounds = true
        self.cheekLabel.layer.cornerRadius = 5
        self.cheekLabel.isUserInteractionEnabled = true
        self.cheekLabel.addGestureRecognizer(UITapGestureRecognizer(target: self, action: Selector(("onTab:"))))
        
        self.noseLabel.layer.masksToBounds = true
        self.noseLabel.layer.cornerRadius = 5
        self.noseLabel.isUserInteractionEnabled = true
        self.noseLabel.addGestureRecognizer(UITapGestureRecognizer(target: self, action: Selector(("onTab:"))))

        self.tongueLabel.layer.masksToBounds = true
        self.tongueLabel.layer.cornerRadius = 5
        self.tongueLabel.isUserInteractionEnabled = true
        self.tongueLabel.addGestureRecognizer(UITapGestureRecognizer(target: self, action: Selector(("onTab:"))))

        // Location nav bar
        self.prevImage.addGestureRecognizer(UITapGestureRecognizer(target: self, action: Selector(("onPrev:"))))
        self.prevImage.isUserInteractionEnabled = true
        self.nextImage.addGestureRecognizer(UITapGestureRecognizer(target: self, action: Selector(("onNext:"))))
        self.nextImage.isUserInteractionEnabled = true
        
        //
        rangeSlider.delegate = self
        for loc in BLENDSHAPE_LOCATIONS {
            ranges.append(Range(name: loc))
        }
        
        self.update()
    }

    func update()
    {
        // tab
        self.eyeLabel.backgroundColor = UIColor.clear
        self.jawLabel.backgroundColor = UIColor.clear
        self.mouthLabel.backgroundColor = UIColor.clear
        self.eyebrowLabel.backgroundColor = UIColor.clear
        self.cheekLabel.backgroundColor = UIColor.clear
        self.noseLabel.backgroundColor = UIColor.clear
        self.tongueLabel.backgroundColor = UIColor.clear
        
        if(self.index >= self.eyeIndex && self.index < self.jawIndex) {
            self.eyeLabel.backgroundColor = UIColor.gray
        }
        else if(self.index >= self.jawIndex && self.index < self.mouthIndex) {
            self.jawLabel.backgroundColor = UIColor.gray
        }
        else if(self.index >= self.mouthIndex && self.index < self.eyebrowIndex) {
            self.mouthLabel.backgroundColor = UIColor.gray
        }
        else if(self.index >= self.eyebrowIndex && self.index < self.cheekIndex) {
            self.eyebrowLabel.backgroundColor = UIColor.gray
        }
        else if(self.index >= self.cheekIndex && self.index < self.noseIndex) {
            self.cheekLabel.backgroundColor = UIColor.gray
        }
        else if(self.index >= self.noseIndex && self.index < self.tongueIndex) {
            self.noseLabel.backgroundColor = UIColor.gray
        }
        else {
            self.tongueLabel.backgroundColor = UIColor.gray
        }
        
        // label
        self.currentLabel.text = BLENDSHAPE_LOCATIONS[ self.index ]
        
        // slider
        if let range = self.ranges.first(where: {$0.name == BLENDSHAPE_LOCATIONS[ self.index ]}) {
            rangeSlider.selectedMinValue = range.min
            rangeSlider.selectedMaxValue = range.max
        } else {
           print("Error: range not found for ", BLENDSHAPE_LOCATIONS[ self.index ])
        }
    }
    
    @IBAction func onPrev(_ sender: UIImageView) {
        self.index -= 1
        if self.index < 0 {
            self.index = BLENDSHAPE_LOCATIONS.count - 1
        }
        
        self.update()
    }
    
    @IBAction func onNext(_ sender: UIImageView) {
        self.index += 1
        if self.index > BLENDSHAPE_LOCATIONS.count - 1 {
            self.index = 0
        }
        
        self.update()
    }
    
    @IBAction func onTab(_ gestureRecognizer : UITapGestureRecognizer) {
        guard gestureRecognizer.view != nil else { return }
        let tab = gestureRecognizer.view!
        switch(tab.tag) {
            case 0: self.index = self.eyeIndex; break;
            case 1: self.index = self.jawIndex; break;
            case 2: self.index = self.mouthIndex; break;
            case 3: self.index = self.eyebrowIndex; break;
            case 4: self.index = self.cheekIndex; break;
            case 5: self.index = self.noseIndex; break;
            case 6: self.index = self.tongueIndex; break;
            default: return
        }
        
        self.update()
    }
    
    // MARK: - RangeSeekSliderDelegate
    
    public func rangeSeekSlider(_ slider: RangeSeekSlider, didChange minValue: CGFloat, maxValue: CGFloat) {
        if slider === rangeSlider {
            if let range = self.ranges.first(where: {$0.name == BLENDSHAPE_LOCATIONS[ self.index ]}) {
                range.min = minValue
                range.max = maxValue
            } else {
               print("Error: range not found for ", BLENDSHAPE_LOCATIONS[ self.index ])
            }
        }
    }

}

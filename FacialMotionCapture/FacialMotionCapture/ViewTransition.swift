//
//  ViewTransition.swift
//  FacialMotionCapture
//
//  Created by Jihyun Lee on 5/9/20.
//  Copyright © 2020 Jihyun Lee. All rights reserved.
//

import Foundation
import UIKit

// MARK:- View Transition

extension ViewController
{
    // MARK: View Transition - Top To Bottom
    class func viewSlideInFromTopToBottom(view:UIView) -> Void {
        let transition:CATransition = CATransition()
        transition.duration = 0.5
        transition.timingFunction = CAMediaTimingFunction(name: CAMediaTimingFunctionName.easeInEaseOut)
        transition.type = CATransitionType.push
        transition.subtype = CATransitionSubtype.fromBottom
        view.layer.add(transition, forKey: kCATransition)
    }
    
    // MARK: View Transition - Bottom To Top
    class func viewSlideInFromBottomToTop(view:UIView) -> Void {
        let transition:CATransition = CATransition()
        transition.duration = 0.5
        transition.timingFunction = CAMediaTimingFunction(name: CAMediaTimingFunctionName.easeInEaseOut)
        transition.type = CATransitionType.push
        transition.subtype = CATransitionSubtype.fromTop
        view.layer.add(transition, forKey: kCATransition)
    }
}

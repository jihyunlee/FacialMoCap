//
//  FileManager.swift
//  FacialMotionCapture
//
//  Created by Jihyun Lee on 5/9/20.
//  Copyright © 2020 Jihyun Lee. All rights reserved.
//

import Foundation

public class _FileManager {
    
    static func generateCurrentTimeStamp () -> String {
        let formatter = DateFormatter()
        formatter.dateFormat = "yyyy_MM_dd_hh_mm_ss"
        return (formatter.string(from: Date()) as NSString) as String
    }
    
    static func writeFile(array: [String]) {
        let paths = FileManager.default.urls(for: .documentDirectory, in: .userDomainMask)
        guard paths.count > 0 else {
            return
        }

        do{
            var data = "["
            data.append(array.joined(separator: ", "))
            data.append("]")

            try data.write(to: paths[0].appendingPathComponent("data_" + generateCurrentTimeStamp() + ".json"), atomically: true, encoding: String.Encoding.utf8)
            
        } catch{
            print("Failed to write file")
        }
    }
    
}

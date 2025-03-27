import os
import numpy as np
import matplotlib
import matplotlib.pyplot as plt
import math
#from scipy.signal import find_peaks

lam = np.linspace(200,800,301)
#lam_at = np.linspace(300,520,111) #wavelength for attenuation length calculation
lam_at = np.linspace(400,700,151)
# Tper_L = ['1', '2', '3']#, '4', '5', '6']
# Tper_L_N = ['1', '2', '3']#,'4', '5', '6']
# Tper_T = ['1', '2', '3']#,'4', '5', '6']
# Tper_L_at = ['1', '2', '3']#,'4', '5', '6']
# Tper_L_at_N = ['1', '2', '3']#,'4', '5', '6']
# Tper_T_at = ['1', '2', '3']#,'4', '5', '6']
# AtLen = ['1', '2', '3']#,'4', '5', '6']
# lam_At=['1', '2', '3']#, '4', '5', '6']
Tper_L = ['1', '2', '3', '4', '5', '6', '7', '8']
Tper_L_N = ['1', '2', '3','4', '5', '6', '7', '8']
Tper_T = ['1', '2', '3','4', '5', '6', '7', '8']
Tper_L_at = ['1', '2', '3','4', '5', '6', '7', '8']#, '7', '8', '9'
Tper_L_at_N = ['1', '2', '3','4', '5', '6', '7', '8']
Tper_T_at = ['1', '2', '3','4', '5', '6', '7', '8']
AtLen = ['1', '2', '3','4', '5', '6', '7', '8']
lam_At=['1', '2', '3', '4', '5', '6', '7', '8']

L_l = 16 #cm
L_t = 2.2 #cm
#for i in {1, 2, 3}:
for i in {1,2,3,4,5,6,7,8}:
    Tper_L[i-1] = []
    Tper_L_at[i-1] = []
    #print(Tper_L[1])
    Tper_T[i-1] = []
    Tper_T_at[i-1] = []
    AtLen[i-1] = []

            
    #Transverse for Transmission

    with open('B'+str(i)+'TT3.ASP', 'r') as file:
        for line_number, line in enumerate(file, start=1):
            if line_number >= 7 and line_number < 308:
            #if line_number >= 7 and line_number < 258:
                line =line.strip()
                try:
                # print(lin)
                    perT = float(line.strip())
             #       print(line_number, line)
                    Tper_T[i-1].append(perT)


                except ValueError:
                    print(f"skipping invalid line: {line}")
                    
    #Longitudinal for Transmission
    with open('B'+str(i)+'LT3.ASP', 'r') as file:
        for line_number, line in enumerate(file, start=1):
            if 7 <= line_number < 308:
            #if line_number >= 7 and line_number < 258:
                line =line.strip()
                try:
                # print(lin)
                    perL = float(line.strip())
             #       print(line_number, line)
                    Tper_L[i-1].append(perL)
                except ValueError:
                    print(f"skipping invalid line: {line}")
                    continue 

        # # Normalize with respect to transmission if applicable
        # if Tper_T[i-1][300] < Tper_L[i-1][300]:
        #     for j in list(range(301)):
        #         Tper_L_N[i-1][j] = Tper_L[i-1][j]*Tper_T[i-1][300] / Tper_L[i-1][300]
            
                    
        # else:
        #     Tper_L_N[i-1] = Tper_L[i-1]
                
                
    #FOR ATTENUATION
    #--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    #Transverse 

    with open('B'+str(i)+'TT3.ASP', 'r') as file:#kya hua
        for line_number, line in enumerate(file, start=1):
            #if line_number >= 57 and line_number < 168:
            if line_number >= 107 and line_number < 258: #index numbers corresponds to lambda = 400 and 702 nm.
                line =line.strip()
                try:
                # print(lin)
                    perT_at = float(line.strip())
             #       print(line_number, line)
                    Tper_T_at[i-1].append(perT_at)


                except ValueError:
                    print(f"skipping invalid line: {line}")
    
    Tper_L_at_N[i-1]= []
    #LONGITUDINAL
    #-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- 
    # #Storing without Normalization                  
    
    # with open('B'+str(i)+'LT3.ASP', 'r') as file:
    #     for line_number, line in enumerate(file, start=1):
    #         #if line_number >= 57 and line_number < 168:
    #         if line_number >= 107 and line_number < 258:#index numbers corresponds to lambda = 400 and 702 nm.
    #             line =line.strip()
    #             try:
    #             # print(lin)
    #                 perL_at = float(line.strip())
    #          #       print(line_number, line)
    #                 #if perL_at < Tper_T_at[i-1][line_number-107]:
    #                 Tper_L_at_N[i-1].append(perL_at)
    #                 #else:
                        

    #             except ValueError:
    #                 print(f"skipping invalid line: {line}")
    #             #print(line_number)
    #     print("Stored without normalization") 
        
    #-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- 
    #NORMALIZATION WRT THE FIRST INTERSECTION
    print(i)
    with open('B'+str(i)+'LT3.ASP', 'r') as file:
        for line_number, line in enumerate(file, start=1):
            #if line_number >= 57 and line_number < 168:
            #print(line_number)
            if line_number >= 107 and line_number < 258:#index numbers corresponds to lambda = 400 and 702 nm.
                line =line.strip()
                ln=line_number
                try:
                # print(lin)
                    #print(line)
                    perL_at = float(line.strip())
                    # print(ln)
                    # print(perL_at)
                    # print(Tper_T_at[i-1][ln-107])
            #       print(line_number, line)
                    if perL_at < Tper_T_at[i-1][ln-107]:
                        Tper_L_at[i-1].append(perL_at)
                    else:
                        #print(line_number)
                        Tper_L_at[i-1].append(perL_at)
                        break
                    #print(line_number)   
                except ValueError:
                    print(f"skipping invalid line: {line}")
                
          
    norm_index=ln-107     
    print(norm_index)        
    with open('B'+str(i)+'LT3.ASP', 'r') as file: 
        #print(norm_index)
        if perL_at > Tper_T_at[i-1][norm_index]:
            
           
            for line_number, line_N in enumerate(file, start=1):
            #if line_number >= 57 and line_number < 168:
                if line_number >= 107 and line_number < 258:#index numbers corresponds to lambda = 400 and 702 nm.
                    line_N =line_N.strip()
                    
                    try:
                        if norm_index != 7:
                            perL_at_N = float(line_N.strip())* Tper_T_at[i-1][norm_index]/Tper_L_at[i-1][norm_index]
                            Tper_L_at_N[i-1].append(perL_at_N)
                        else:
                            perL_at_N = float(line_N.strip())
                    #print(line_number)   
                    except ValueError:
                       # print("here")
                        print(f"skipping invalid line: {line}")
        else: 
            Tper_L_at_N[i-1] = Tper_L_at[i-1]
    print("Stored with first incidence normalization")             
    #print(Tper_L_at[i-1])
    #-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- 
    # #NORMALIZATION WRT THE TRANSVERSE TRANSMITTANCE AT 800 NM

    # with open('B'+str(i)+'LT3.ASP', 'r') as file:
    #     for line_number, line in enumerate(file, start=1):
    #         #if line_number >= 57 and line_number < 168:
    #         if line_number >= 107 and line_number < 258:#index numbers corresponds to lambda = 400 and 702 nm.
    #             line =line.strip()
    #             try:
    #             # print(lin)
    #                 #print(line)
    #                 perL_at = float(line.strip())
    #         #       print(line_number, line)
    #                 if Tper_T[i-1][300] < Tper_L[i-1][300]: #Comparing Longitudinal and transverse Transmittance at 800nm
    #                     Tper_L_at_N[i-1].append(perL_at*Tper_T[i-1][300]/Tper_L[i-1][300])
    #                 else:
    #                     Tper_L_at_N[i-1].append(perL_at)
    #                 #print(line_number)   
    #             except ValueError:
    #                 print(f"skipping invalid line: {line}")
    # print("Stored with transverse transmittence @ 800nm normalization")  
           
    #-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- 
    # Assigning attenuation length values
    lam_At[i-1]=[]
    AtLen[i-1] = []


    m=0
    for t, l in zip(Tper_T_at[i-1], Tper_L_at_N[i-1]):
        if l < t:
            lam_At[i-1].append(float(lam_at[m]))
            value = (L_l - L_t)/math.log(t / l)
            AtLen[i-1].append(value)
            m=m+1
        else:
            print(str(lam_at[m])+ "nm is the cutoff")
            break
    #print(i)    
    #print(AtLen[i-1])
    
    if len(lam_At[i-1]) < lam_At[0].index(478):
        Mes = "can not be found at 495nm"
    else:
        Mes1 = AtLen[i-1][lam_At[i-1].index(478)]
        if len(lam_At[i-1]) < lam_At[0].index(496):
            Mes= "{:.2f}".format(Mes1)
        else:
            Mes2= AtLen[i-1][lam_At[i-1].index(478)]
            Mes = "{:.2f}".format(np.mean([Mes1, Mes2]))
    #AtLen[i-1][lam_At[5].index(494)]
#--------------------------------------------------------------------------------------------------------------------------------------------------------

    fig, (ax1, ax2) = plt.subplots(2, 1, sharex=True, figsize=(8, 6))
    # Plot on the first subplot (ax1)
    ax1.plot(lam, Tper_L[i-1], label='Longitudinal(16cm)')
    ax1.plot(lam, Tper_T[i-1], label='Transverse(2.2cm)')
    ax1.set_ylabel('Transmission (%)')
    ax1.set_title(f'Transmittance BGO ({i})')
    ax1.grid(True)
    ax1.legend()

    # Plot on the second subplot (ax2)
    ax2.plot(lam_At[i-1], AtLen[i-1], label='Attenuation Length (cm)', linestyle='--')
    ax2.set_xlabel('Wavelength (nm)')
    ax2.set_ylabel('Attenuation Length (cm)')
   # ax2.set_ylim(0,2000)
    ax2.grid(True)
    ax2.text(0.0, 0.8, f'Attenuation Length(495nm)={Mes}cm', transform=ax2.transAxes, fontsize=11, verticalalignment='top')
    ax2.legend()
    
    #plt.tight_layout()
    
    
    
    #print(AtLength[i-1])   


#    plt.savefig("BGOT" + str(i) + "NFIn.pdf", format="pdf")
#print(AtLength[4])
#print(Tper)
#print(Tper_L)
plt.show()
plt.ion()
print("Transmissions executed successfully")
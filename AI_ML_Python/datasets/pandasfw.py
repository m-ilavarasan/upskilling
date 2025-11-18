import pandas as pd
a = {'Student':['David', 'Samuel', 'Terry', 'Evan'],
     'Age':['27', '24', '22', '32'],
     'Country':['UK', 'Canada', 'China', 'USA'],
     'Course':['Python','Data Structures','Machine Learning','Web Development'],
     'Marks':['85','72','89','76']}
df1 = pd.DataFrame(a)
df1
    
df1 = pd.DataFrame(a)
print(df1['Student'])
print(df1.iloc[0:2])
print(df1.iloc[0:3])

    
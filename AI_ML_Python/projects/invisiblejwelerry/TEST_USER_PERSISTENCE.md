# User Persistence Test

## Issue Fixed
**Problem**: When switching between users (e.g., signing in as admin), previously created users were being deleted because the `getUsers()` function was replacing the entire users list with just the admin user whenever the list appeared empty.

**Root Cause**: The logic checked `if (users.length === 0)` and then REPLACED all users with just admin. This would wipe out any previously created users.

**Solution**: Changed the logic to check if admin user exists BY ID (`u.id === 'user-admin'`), and if it doesn't exist, APPEND it to the list instead of replacing all users.

---

## Test Steps

### Step 1: Initial Load
1. Open the app in a fresh browser session (clear localStorage if needed)
2. Check browser console: `console.log(JSON.parse(localStorage.getItem('users')))`
3. **Expected Result**: Should see an array with 1 admin user:
```json
[{"id":"user-admin","username":"admin","password":"admin123","isAdmin":true}]
```

### Step 2: Create Test User 1
1. Click "Sign up"
2. Username: `testuser1`
3. Password: `password123`
4. Click "Create account"
5. Check localStorage: `console.log(JSON.parse(localStorage.getItem('users')))`
6. **Expected Result**: Should see 2 users (admin + testuser1):
```json
[
  {"id":"user-admin","username":"admin","password":"admin123","isAdmin":true},
  {"id":"user-...", "username":"testuser1","password":"password123","isAdmin":false}
]
```

### Step 3: Sign Out & Sign In as Admin
1. Click "Sign out"
2. Click "Sign in"
3. Username: `admin`
4. Password: `admin123`
5. Click "Sign in"
6. Check localStorage again: `console.log(JSON.parse(localStorage.getItem('users')))`
7. **Expected Result**: Should STILL see both users (admin + testuser1) - no deletion!

### Step 4: Sign Out & Sign In as Test User
1. Click "Sign out"
2. Click "Sign in"
3. Username: `testuser1`
4. Password: `password123`
5. Click "Sign in"
6. **Expected Result**: Should successfully log in (user not deleted)

### Step 5: Create Test User 2
1. Sign out
2. Click "Sign up"
3. Username: `testuser2`
4. Password: `password456`
5. Click "Create account"
6. Check localStorage: `console.log(JSON.parse(localStorage.getItem('users')))`
7. **Expected Result**: Should see 3 users (admin + testuser1 + testuser2)

### Step 6: Final Verification
1. Sign out and sign in as admin again
2. All 3 users should still exist in localStorage
3. **BUG FIX VERIFIED** ✓

---

## Code Changes Made

File: `app.js`

**Before (Buggy)**:
```javascript
function getUsers() {
  try {
    const data = localStorage.getItem(STORAGE_KEYS.users);
    let users = data ? JSON.parse(data) : [];
    if (users.length === 0) {
      // This REPLACES all users with just admin!
      users = [{...admin...}];
      setUsers(users);
    }
    return users;
  } catch {
    return [];
  }
}
```

**After (Fixed)**:
```javascript
function getUsers() {
  try {
    const data = localStorage.getItem(STORAGE_KEYS.users);
    let users = data ? JSON.parse(data) : [];
    
    // Check if admin user exists BY ID
    const adminExists = users.some(u => u.id === 'user-admin');
    if (!adminExists) {
      // APPEND admin user instead of replacing all users
      users.push({...admin...});
      setUsers(users);
    }
    return users;
  } catch {
    // Handle error gracefully
    const adminUser = [{...admin...}];
    try {
      setUsers(adminUser);
    } catch {}
    return adminUser;
  }
}
```

---

## Key Differences
1. **From**: `if (users.length === 0)` → **To**: `if (!adminExists)`
2. **From**: `users = [{...}]` (replacement) → **To**: `users.push({...})` (append)
3. **From**: ID comparison not used → **To**: `users.some(u => u.id === 'user-admin')`
4. **Result**: All users are preserved when switching between accounts ✓

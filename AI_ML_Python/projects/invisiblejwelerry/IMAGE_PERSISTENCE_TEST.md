# Image Persistence Test Guide

## Issue Fixed
**Problem**: When editing a product without uploading a new image, the existing image was being deleted/replaced with `undefined`.

**Root Cause**: In the edit form submission, the code was using `editFormImageData || undefined`, which would delete the image if no new one was uploaded.

**Solution**: Changed to `editFormImageData || products[idx].imageUrl` to preserve the existing image when no new image is uploaded.

---

## Test Steps

### Step 1: Add Product with Image
1. **Sign in as Admin**
   - Username: `admin`
   - Password: `admin123`
   - Click "Management" tab

2. **Add a New Product with Image**
   - Product Name: `Test Ring`
   - Click the image upload area
   - Select any image from your computer
   - Price: Leave blank (default 99)
   - Click "Add Product"
   - **Expected**: Product appears in the table with the image thumbnail
   - **Open DevTools** (F12) → Console:
     ```javascript
     let prod = JSON.parse(localStorage.getItem('products')).find(p => p.name === 'Test Ring');
     console.log('Image URL exists:', !!prod.imageUrl);
     console.log('Image URL length:', prod.imageUrl?.length);
     ```
   - **Expected Result**: Image URL exists and has significant length (thousands of characters for base64 data)

### Step 2: Page Refresh - Verify Image Persists
1. **Reload the page** (F5 or Ctrl+R)
2. **Sign in as Admin** again
3. **Go to Management tab**
4. **Check the "Test Ring" product**
   - **Expected**: Image thumbnail is still visible
   - **Run in Console**:
     ```javascript
     let prod = JSON.parse(localStorage.getItem('products')).find(p => p.name === 'Test Ring');
     console.log('Image still there:', !!prod.imageUrl);
     ```
   - **Expected Result**: `Image still there: true`

### Step 3: Edit Product Without Changing Image
1. **Click "Edit" button on Test Ring product**
2. **Change the name** to `Updated Ring`
3. **Don't upload a new image** - just leave it as is
4. **Click "Save"**
5. **Check the product** in the management table
   - **Expected**: Image is STILL VISIBLE (not deleted!)
   - **Run in Console**:
     ```javascript
     let prod = JSON.parse(localStorage.getItem('products')).find(p => p.name === 'Updated Ring');
     console.log('Image after edit without upload:', !!prod.imageUrl);
     console.log('Image size:', prod.imageUrl?.length);
     ```
   - **Expected Result**: Image persists with same size as before

### Step 4: Edit Product AND Change Image
1. **Click "Edit" button** again
2. **Change name** to `New Ring`
3. **Upload a NEW image** (different from before)
4. **Click "Save"**
5. **Check the product**
   - **Expected**: NEW image is displayed
   - **Run in Console**:
     ```javascript
     let prod = JSON.parse(localStorage.getItem('products')).find(p => p.name === 'New Ring');
     console.log('New image exists:', !!prod.imageUrl);
     ```
   - **Expected Result**: `New image exists: true`

### Step 5: Sign Out & Sign In - Verify Persistence
1. **Click "Sign out"**
2. **Go to Home page** → Products should still show the image thumbnail
3. **Sign in as DIFFERENT user** (or create a test user and sign in)
4. **Switch back to Admin** by signing out and signing in as admin
5. **Check Management tab**
   - **Expected**: All product images are STILL THERE
   - This tests that images aren't lost when switching between users

### Step 6: Test Home Page Display
1. **Go to Home tab**
2. **Check all products display with images**
   - **Expected**: All products show image thumbnails
3. **Click on product image** to open modal
   - **Expected**: Full image displays in modal
4. **Close modal** and refresh page (F5)
5. **Check Home page again**
   - **Expected**: Images still visible and clickable

---

## Code Changes Made

### File: `app.js` - Edit Form Submission

**Before (Bug)**:
```javascript
// If user didn't upload new image, editFormImageData is null
// This would DELETE the existing image!
products[idx] = { ...products[idx], name, imageUrl: editFormImageData || undefined, price, active };
```

**After (Fixed)**:
```javascript
// If no new image, keep the existing one
const imageUrl = editFormImageData || products[idx].imageUrl;
products[idx] = { ...products[idx], name, imageUrl: imageUrl || undefined, price, active };
```

---

## How Image Storage Works

1. **Image Upload**: 
   - User selects image → compressed to max 400px
   - Stored as Base64 data URL string
   - Stored in product object: `{ imageUrl: "data:image/jpeg;base64,..." }`

2. **Persistence**:
   - Product object saved to localStorage as JSON
   - Image (base64 string) is part of the JSON data
   - Survives page refresh!

3. **Display**:
   - On page load, `getProducts()` retrieves from localStorage
   - Products with images display thumbnails in tables
   - Images show in product cards on home page

---

## Verification Checklist

- [ ] Can add product with image
- [ ] Image persists after page refresh
- [ ] Can edit product name without losing image
- [ ] Can replace image on existing product
- [ ] All images show on Home page
- [ ] Images work in management table
- [ ] Images survive switching between user accounts
- [ ] Image modal opens and displays correctly
- [ ] Images are stored as Base64 in localStorage

---

## Notes

- Images are stored as Base64 strings in localStorage
- Each image can be 50-200KB of text data
- localStorage has ~5-10MB limit (plenty for many products)
- Images are NOT hosted - they're stored client-side
- Clearing browser cache/localStorage will delete images (expected behavior)
- Each product can have one image

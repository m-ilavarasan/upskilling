// Invisible Jewellery - App State & Routing
const STORAGE_KEYS = {
  products: 'products',
  cart: 'cart',
  orders: 'orders',
  users: 'users',
  currentUser: 'currentUser',
  pendingCheckout: 'pendingCheckout'
};

const DEFAULT_PRICE = 99;
const GST_RATE = 0;

// Views
const views = ['home', 'cart', 'management', 'reports', 'signin', 'signup'];

function showView(viewId) {
  views.forEach(v => {
    const el = document.getElementById(`${v}-view`);
    const link = document.querySelector(`[data-view="${v}"]`);
    if (el) el.classList.toggle('active', v === viewId);
    if (link) link.classList.toggle('active', v === viewId);
  });
}

function initNavigation() {
  document.querySelectorAll('.nav-link').forEach(link => {
    link.addEventListener('click', (e) => {
      e.preventDefault();
      const view = link.dataset.view;
      if (view) showView(view);
      if (view === 'home') renderProducts();
      if (view === 'cart') renderCart();
      if (view === 'management') renderManagement();
      if (view === 'reports') renderReports();
      if (view === 'signin' || view === 'signup') {
        // auth views don't need extra rendering hooks
      }
    });
  });

  const signOutLink = document.getElementById('nav-signout');
  if (signOutLink) {
    signOutLink.addEventListener('click', (e) => {
      e.preventDefault();
      setCurrentUser(null);
      localStorage.removeItem(STORAGE_KEYS.pendingCheckout);
      applyAuthRestrictions();
      showView('home');
    });
  }
}

// Product model & localStorage
const SAMPLE_PRODUCTS = [
  { id: 'prod-sample-1', name: 'Invisible Necklace', price: 99, imageUrl: 'https://via.placeholder.com/400x400/8b7355/ffffff?text=Necklace', active: true },
  { id: 'prod-sample-2', name: 'Invisible Bracelet', price: 99, imageUrl: 'https://via.placeholder.com/400x400/6b5344/ffffff?text=Bracelet', active: true },
  { id: 'prod-sample-3', name: 'Invisible Earrings', price: 99, imageUrl: 'https://via.placeholder.com/400x400/8b7355/ffffff?text=Earrings', active: true },
  { id: 'prod-sample-4', name: 'Invisible Ring', price: 99, imageUrl: 'https://via.placeholder.com/400x400/6b5344/ffffff?text=Ring', active: true },
  { id: 'prod-sample-5', name: 'Invisible Pendant', price: 99, imageUrl: 'https://via.placeholder.com/400x400/8b7355/ffffff?text=Pendant', active: true },
  { id: 'prod-sample-6', name: 'Invisible Anklet', price: 99, imageUrl: 'https://via.placeholder.com/400x400/6b5344/ffffff?text=Anklet', active: true }
];

function getProducts() {
  try {
    const data = localStorage.getItem(STORAGE_KEYS.products);
    let products = data ? JSON.parse(data) : [];
    if (products.length === 0) {
      products = [...SAMPLE_PRODUCTS];
      setProducts(products);
    }
    return products;
  } catch {
    return SAMPLE_PRODUCTS;
  }
}

function setProducts(products) {
  localStorage.setItem(STORAGE_KEYS.products, JSON.stringify(products));
}

function getProductById(id) {
  return getProducts().find(p => String(p.id) === String(id));
}

// Simple auth (localStorage, demo only)
function getUsers() {
  try {
    const data = localStorage.getItem(STORAGE_KEYS.users);
    let users = data ? JSON.parse(data) : [];

    // Check if admin user exists, if not add it
    const adminExists = users.some(u => u.id === 'user-admin');
    if (!adminExists) {
      users.push({
        id: 'user-admin',
        username: 'admin',
        password: 'admin123',
        isAdmin: true
      });
      setUsers(users);
    }
    return users;
  } catch {
    // If error parsing, return only admin user
    const adminUser = [{
      id: 'user-admin',
      username: 'admin',
      password: 'admin123',
      isAdmin: true
    }];
    try {
      setUsers(adminUser);
    } catch { }
    return adminUser;
  }
}

function setUsers(users) {
  localStorage.setItem(STORAGE_KEYS.users, JSON.stringify(users));
}

function getCurrentUser() {
  try {
    const data = localStorage.getItem(STORAGE_KEYS.currentUser);
    return data ? JSON.parse(data) : null;
  } catch {
    return null;
  }
}

function setCurrentUser(user) {
  if (!user) {
    localStorage.removeItem(STORAGE_KEYS.currentUser);
    return;
  }
  const minimal = { id: user.id, username: user.username, isAdmin: !!user.isAdmin };
  localStorage.setItem(STORAGE_KEYS.currentUser, JSON.stringify(minimal));
}

function applyAuthRestrictions() {
  const user = getCurrentUser();
  const isAdmin = !!(user && user.isAdmin);

  const managementLi = document.querySelector('[data-view="management"]')?.parentElement;
  const reportsLi = document.querySelector('[data-view="reports"]')?.parentElement;
  if (managementLi) managementLi.style.display = isAdmin ? '' : 'none';
  if (reportsLi) reportsLi.style.display = isAdmin ? '' : 'none';

  const signInLi = document.getElementById('nav-signin')?.parentElement;
  const signUpLi = document.getElementById('nav-signup')?.parentElement;
  const signOutLi = document.getElementById('nav-signout')?.parentElement;
  const navUser = document.getElementById('nav-user');

  if (user) {
    if (signInLi) signInLi.style.display = 'none';
    if (signUpLi) signUpLi.style.display = 'none';
    if (signOutLi) signOutLi.style.display = '';
    if (navUser) {
      navUser.textContent = `Hi, ${user.username}`;
      navUser.style.display = 'inline-block';
    }
  } else {
    if (signInLi) signInLi.style.display = '';
    if (signUpLi) signUpLi.style.display = '';
    if (signOutLi) signOutLi.style.display = 'none';
    if (navUser) {
      navUser.textContent = '';
      navUser.style.display = 'none';
    }
  }

  // If non-admin is on a restricted view, send them home
  const currentView = views.find(v => {
    const el = document.getElementById(`${v}-view`);
    return el && el.classList.contains('active');
  });
  if (!isAdmin && (currentView === 'management' || currentView === 'reports')) {
    showView('home');
  }
}

// Home - Product cards
function renderProducts() {
  const grid = document.getElementById('products-grid');
  if (!grid) return;

  const products = getProducts().filter(p => p.active !== false);

  if (products.length === 0) {
    grid.innerHTML = '<p class="empty-state">No products yet. Add some in Management.</p>';
    return;
  }

  grid.innerHTML = products.map(p => `
    <div class="product-card" data-product-id="${p.id}">
      <div class="product-image">
        <img src="${p.imageUrl || 'https://via.placeholder.com/200x200?text=No+Image'}" alt="${escapeHtml(p.name)}" loading="lazy" onerror="this.src='https://via.placeholder.com/200x200?text=No+Image'">
      </div>
      <div class="product-info">
        <h3 class="product-name">${escapeHtml(p.name)}</h3>
        <p class="product-price">Rs. ${p.price ?? DEFAULT_PRICE}</p>
        <button class="btn btn-primary add-to-cart" data-product-id="${p.id}">Add to Cart</button>
      </div>
    </div>
  `).join('');

  grid.querySelectorAll('.add-to-cart').forEach(btn => {
    btn.addEventListener('click', () => addToCart(btn.dataset.productId));
  });

  grid.querySelectorAll('.product-image img').forEach(img => {
    img.addEventListener('click', () => {
      openImageModal(img.src, img.alt);
    });
  });
}

function escapeHtml(str) {
  const div = document.createElement('div');
  div.textContent = str;
  return div.innerHTML;
}

function addToCart(productId) {
  let cart = JSON.parse(localStorage.getItem(STORAGE_KEYS.cart) || '[]');
  const existing = cart.find(item => String(item.productId) === String(productId));
  if (existing) {
    existing.quantity += 1;
  } else {
    cart.push({ productId, quantity: 1 });
  }
  localStorage.setItem(STORAGE_KEYS.cart, JSON.stringify(cart));
  updateCartBadge();
}

// Cart model
function getCart() {
  try {
    const data = localStorage.getItem(STORAGE_KEYS.cart);
    return data ? JSON.parse(data) : [];
  } catch {
    return [];
  }
}

function setCart(cart) {
  localStorage.setItem(STORAGE_KEYS.cart, JSON.stringify(cart));
  updateCartBadge();
}

function updateCartQuantity(productId, quantity) {
  let cart = getCart();
  const item = cart.find(i => String(i.productId) === String(productId));
  if (!item) return;
  if (quantity <= 0) {
    cart = cart.filter(i => String(i.productId) !== String(productId));
  } else {
    item.quantity = quantity;
  }
  setCart(cart);
}

// Cart page
function renderCart() {
  const cartItemsEl = document.getElementById('cart-items');
  const cartTotalsEl = document.getElementById('cart-totals');

  if (!cartItemsEl || !cartTotalsEl) return;

  const cart = getCart();
  const products = getProducts();
  const productsById = Object.fromEntries(products.map(p => [String(p.id), p]));

  if (cart.length === 0) {
    cartItemsEl.innerHTML = '<p class="empty-state">Your cart is empty.</p>';
    cartTotalsEl.innerHTML = '';
    return;
  }

  let subtotal = 0;
  const rows = cart.map(item => {
    const product = productsById[String(item.productId)];
    const price = product ? (product.price ?? DEFAULT_PRICE) : DEFAULT_PRICE;
    const lineTotal = price * item.quantity;
    subtotal += lineTotal;
    return { ...item, product, price, lineTotal };
  });

  const gstAmount = subtotal * GST_RATE;
  const total = subtotal + gstAmount;

  cartItemsEl.innerHTML = `
    <table class="cart-table">
      <thead>
        <tr>
          <th>Image</th>
          <th>Product</th>
          <th>Price</th>
          <th>Qty</th>
          <th>Total</th>
          <th></th>
        </tr>
      </thead>
      <tbody>
        ${rows.map(r => `
          <tr data-product-id="${r.productId}">
            <td><img src="${r.product?.imageUrl || 'https://via.placeholder.com/50?text=No+Image'}" alt="" class="cart-thumb" onerror="this.src='https://via.placeholder.com/50?text=No+Image'"></td>
            <td>${escapeHtml(r.product?.name || 'Unknown')}</td>
            <td>Rs. ${r.price}</td>
            <td>
              <input type="number" class="cart-qty" value="${r.quantity}" min="1" data-product-id="${r.productId}">
            </td>
            <td>Rs. ${r.lineTotal}</td>
            <td><button class="btn btn-danger cart-remove" data-product-id="${r.productId}">Remove</button></td>
          </tr>
        `).join('')}
      </tbody>
    </table>
  `;

  cartTotalsEl.innerHTML = `
    <div class="totals-row">
      <span>Subtotal:</span>
      <span>Rs. ${subtotal}</span>
    </div>
    <div class="totals-row">
      <span>GST (${GST_RATE * 100}%):</span>
      <span>Rs. ${gstAmount}</span>
    </div>
    <div class="totals-row total">
      <span>Total:</span>
      <span>Rs. ${total}</span>
    </div>
    <button id="confirm-purchase" class="btn btn-primary" style="margin-top: 0.75rem; width: 100%;">Confirm Purchase</button>
  `;

  // Event listeners
  cartItemsEl.querySelectorAll('.cart-qty').forEach(input => {
    input.addEventListener('change', () => {
      const qty = parseInt(input.value, 10) || 0;
      updateCartQuantity(input.dataset.productId, qty);
      renderCart();
    });
  });

  cartItemsEl.querySelectorAll('.cart-remove').forEach(btn => {
    btn.addEventListener('click', () => {
      updateCartQuantity(btn.dataset.productId, 0);
      renderCart();
    });
  });

  const confirmPurchaseBtn = cartTotalsEl.querySelector('#confirm-purchase');
  if (confirmPurchaseBtn) {
    confirmPurchaseBtn.onclick = () => handleConfirmPurchase();
  }
}

function handleConfirmPurchase() {
  const user = getCurrentUser();
  if (!user) {
    // Not signed in: remember intent and send to sign-in page
    localStorage.setItem(STORAGE_KEYS.pendingCheckout, 'true');
    showView('signin');
    return;
  }
  // Signed in: proceed with checkout
  checkout();
}

function checkout() {
  const cart = getCart();
  if (cart.length === 0) return;

  const products = getProducts();
  const productsById = Object.fromEntries(products.map(p => [String(p.id), p]));

  const items = cart.map(item => {
    const product = productsById[String(item.productId)];
    const unitPrice = product ? (product.price ?? DEFAULT_PRICE) : DEFAULT_PRICE;
    return { productId: item.productId, quantity: item.quantity, unitPrice };
  });

  const subtotal = items.reduce((sum, i) => sum + i.unitPrice * i.quantity, 0);
  const gstAmount = subtotal * GST_RATE;
  const total = subtotal + gstAmount;

  const order = {
    id: 'ord-' + Date.now(),
    timestamp: new Date().toISOString(),
    items,
    subtotal,
    gstRate: GST_RATE,
    gstAmount,
    total
  };

  const orders = JSON.parse(localStorage.getItem(STORAGE_KEYS.orders) || '[]');
  orders.push(order);
  localStorage.setItem(STORAGE_KEYS.orders, JSON.stringify(orders));

  setCart([]);
  showView('home');
  renderCart();
  renderProducts();
  alert('Thank you! Your order has been placed.');
}

// Full image modal for product images
function openImageModal(src, altText) {
  const overlay = document.createElement('div');
  overlay.className = 'modal-overlay';
  overlay.innerHTML = `
    <div class="modal image-modal">
      <img src="${src}" alt="${escapeHtml(altText || '')}" class="image-modal-img">
    </div>
  `;
  document.body.appendChild(overlay);

  overlay.onclick = (e) => {
    if (e.target === overlay) {
      overlay.remove();
    }
  };
}

// Auth forms
function initAuthForms() {
  const signinForm = document.getElementById('signin-form');
  const signinError = document.getElementById('signin-error');
  if (signinForm) {
    signinForm.onsubmit = (e) => {
      e.preventDefault();
      const username = document.getElementById('signin-username')?.value?.trim();
      const password = document.getElementById('signin-password')?.value || '';
      if (!username || !password) return;

      const users = getUsers();
      const user = users.find(u => u.username === username && u.password === password);
      if (!user) {
        if (signinError) {
          signinError.textContent = 'Invalid username or password.';
        }
        return;
      }

      if (signinError) signinError.textContent = '';
      setCurrentUser(user);
      applyAuthRestrictions();
      const hadPendingCheckout = localStorage.getItem(STORAGE_KEYS.pendingCheckout) === 'true';
      localStorage.removeItem(STORAGE_KEYS.pendingCheckout);
      if (hadPendingCheckout && getCart().length > 0) {
        showView('cart');
        renderCart();
      } else {
        showView('home');
      }
    };
  }

  const signupForm = document.getElementById('signup-form');
  const signupError = document.getElementById('signup-error');
  if (signupForm) {
    signupForm.onsubmit = (e) => {
      e.preventDefault();
      const username = document.getElementById('signup-username')?.value?.trim();
      const password = document.getElementById('signup-password')?.value || '';
      if (!username || !password) return;

      const users = getUsers();
      if (users.some(u => u.username === username)) {
        if (signupError) {
          signupError.textContent = 'Username already exists. Please choose another.';
        }
        return;
      }

      const newUser = {
        id: 'user-' + Date.now(),
        username,
        password,
        isAdmin: false
      };
      users.push(newUser);
      setUsers(users);
      if (signupError) signupError.textContent = '';
      setCurrentUser(newUser);
      applyAuthRestrictions();
      const hadPendingCheckout = localStorage.getItem(STORAGE_KEYS.pendingCheckout) === 'true';
      localStorage.removeItem(STORAGE_KEYS.pendingCheckout);
      if (hadPendingCheckout && getCart().length > 0) {
        showView('cart');
        renderCart();
      } else {
        showView('home');
      }
    };
  }
}

// Management - CRUD
function generateProductId() {
  return 'prod-' + Date.now() + '-' + Math.random().toString(36).slice(2, 8);
}

function compressImage(file, maxSize = 400) {
  return new Promise((resolve, reject) => {
    const reader = new FileReader();
    reader.onload = (e) => {
      const img = new Image();
      img.onload = () => {
        try {
          const canvas = document.createElement('canvas');
          let { width, height } = img;
          if (width > height && width > maxSize) {
            height = (height * maxSize) / width;
            width = maxSize;
          } else if (height > maxSize) {
            width = (width * maxSize) / height;
            height = maxSize;
          }
          canvas.width = width;
          canvas.height = height;
          const ctx = canvas.getContext('2d');
          ctx.drawImage(img, 0, 0, width, height);
          resolve(canvas.toDataURL('image/jpeg', 0.85));
        } catch {
          resolve(e.target.result);
        }
      };
      img.onerror = () => resolve(e.target.result);
      img.src = e.target.result;
    };
    reader.onerror = () => reject(new Error('Failed to read file'));
    reader.readAsDataURL(file);
  });
}

function setupImageUpload(previewEl, fileInput, onImageSelected) {
  if (!previewEl || !fileInput) return;
  previewEl.onclick = (e) => { if (e.target !== fileInput) fileInput.click(); };
  fileInput.onclick = (e) => e.stopPropagation();
  fileInput.onchange = async () => {
    const file = fileInput.files?.[0];
    if (!file || !file.type.startsWith('image/')) return;
    try {
      const dataUrl = await compressImage(file);
      onImageSelected(dataUrl);
      const placeholder = previewEl.querySelector('.image-placeholder');
      if (placeholder) placeholder.remove();
      const oldImg = previewEl.querySelector('.preview-img');
      if (oldImg) oldImg.remove();
      const img = document.createElement('img');
      img.src = dataUrl;
      img.alt = 'Preview';
      img.className = 'preview-img';
      previewEl.insertBefore(img, fileInput);
      previewEl.classList.add('has-image');
    } catch (err) {
      alert('Failed to load image. Please try another file.');
    }
  };
}

function renderManagement() {
  const container = document.getElementById('management-products');
  const form = document.getElementById('add-product-form');
  if (!container || !form) return;

  let addFormImageData = null;

  const previewEl = document.getElementById('add-image-preview');
  const fileInput = document.getElementById('product-image-file');
  setupImageUpload(previewEl, fileInput, (dataUrl) => { addFormImageData = dataUrl; });

  form.onsubmit = (e) => {
    e.preventDefault();
    const name = document.getElementById('product-name')?.value?.trim();
    const priceInput = document.getElementById('product-price')?.value;
    const price = priceInput === '' || priceInput === null ? DEFAULT_PRICE : Math.max(0, parseFloat(priceInput) || DEFAULT_PRICE);

    if (!name) return;

    const products = getProducts();
    products.push({
      id: generateProductId(),
      name,
      imageUrl: addFormImageData || undefined,
      price,
      active: true
    });
    setProducts(products);

    form.reset();
    addFormImageData = null;
    if (previewEl) {
      previewEl.innerHTML = '<span class="image-placeholder">Click to browse & select image</span>';
      previewEl.classList.remove('has-image');
      const newInput = document.createElement('input');
      newInput.type = 'file';
      newInput.id = 'product-image-file';
      newInput.accept = 'image/*';
      newInput.className = 'image-file-input';
      previewEl.appendChild(newInput);
      setupImageUpload(previewEl, newInput, (dataUrl) => { addFormImageData = dataUrl; });
    }
    document.getElementById('product-price')?.setAttribute('placeholder', 'Price (default 99)');
    renderManagement();
    renderProducts();
  };

  const products = getProducts();

  if (products.length === 0) {
    container.innerHTML = '<p class="empty-state">No products. Add one above.</p>';
    return;
  }

  container.innerHTML = `
    <table class="management-table">
      <thead>
        <tr>
          <th>Image</th>
          <th>Name</th>
          <th>Price</th>
          <th>Active</th>
          <th>Actions</th>
        </tr>
      </thead>
      <tbody>
        ${products.map(p => `
          <tr data-product-id="${p.id}" class="${p.active === false ? 'inactive' : ''}">
            <td><img src="${p.imageUrl || 'https://via.placeholder.com/50?text=No+Image'}" alt="" class="mgmt-thumb" onerror="this.src='https://via.placeholder.com/50?text=No+Image'"></td>
            <td class="mgmt-name">${escapeHtml(p.name)}</td>
            <td class="mgmt-price">Rs. ${p.price ?? DEFAULT_PRICE}</td>
            <td class="mgmt-active">${p.active !== false ? 'Yes' : 'No'}</td>
            <td>
              <button class="btn btn-secondary mgmt-edit" data-product-id="${p.id}">Edit</button>
              <button class="btn btn-danger mgmt-delete" data-product-id="${p.id}">Delete</button>
            </td>
          </tr>
        `).join('')}
      </tbody>
    </table>
  `;

  container.querySelectorAll('.mgmt-edit').forEach(btn => {
    btn.addEventListener('click', () => openEditModal(btn.dataset.productId));
  });

  container.querySelectorAll('.mgmt-delete').forEach(btn => {
    btn.addEventListener('click', () => deleteProduct(btn.dataset.productId));
  });
}

function openEditModal(productId) {
  const product = getProductById(productId);
  if (!product) return;

  let editFormImageData = product.imageUrl || null;

  const modal = document.createElement('div');
  modal.className = 'modal-overlay';
  const hasImage = !!(product.imageUrl);
  modal.innerHTML = `
    <div class="modal">
      <h2>Edit Product</h2>
      <form id="edit-product-form">
        <label>Name</label>
        <input type="text" id="edit-name" value="${escapeHtml(product.name)}" required>
        <label>Product Image</label>
        <div id="edit-image-preview" class="image-preview modal-image-preview ${hasImage ? 'has-image' : ''}" title="Click to upload image">
          <span class="image-placeholder">Click to browse & select image</span>
          <input type="file" id="edit-image-file" accept="image/*" class="image-file-input">
        </div>
        <label>Price</label>
        <input type="number" id="edit-price" value="${product.price ?? DEFAULT_PRICE}" min="0" step="0.01">
        <label><input type="checkbox" id="edit-active" ${product.active !== false ? 'checked' : ''}> Active</label>
        <div class="modal-actions">
          <button type="button" class="btn btn-secondary modal-cancel">Cancel</button>
          <button type="submit" class="btn btn-primary">Save</button>
        </div>
      </form>
    </div>
  `;

  document.body.appendChild(modal);

  const editPreview = modal.querySelector('#edit-image-preview');
  const editFileInput = modal.querySelector('#edit-image-file');
  if (product.imageUrl) {
    const placeholder = editPreview.querySelector('.image-placeholder');
    if (placeholder) placeholder.remove();
    const img = document.createElement('img');
    img.src = product.imageUrl;
    img.alt = 'Current';
    img.className = 'preview-img';
    img.onerror = () => { img.style.display = 'none'; };
    editPreview.insertBefore(img, editPreview.firstChild);
    editPreview.classList.add('has-image');
  }
  setupImageUpload(editPreview, editFileInput, (dataUrl) => { editFormImageData = dataUrl; });

  const cancel = () => {
    modal.remove();
  };

  modal.querySelector('.modal-cancel').onclick = cancel;
  modal.onclick = (e) => { if (e.target === modal) cancel(); };

  modal.querySelector('#edit-product-form').onsubmit = (e) => {
    e.preventDefault();
    const name = document.getElementById('edit-name')?.value?.trim();
    const price = parseFloat(document.getElementById('edit-price')?.value) || DEFAULT_PRICE;
    const active = document.getElementById('edit-active')?.checked !== false;

    if (!name) return;

    const products = getProducts();
    const idx = products.findIndex(p => String(p.id) === String(productId));
    if (idx >= 0) {
      // Keep existing image if no new image was uploaded
      const imageUrl = editFormImageData || products[idx].imageUrl;
      products[idx] = { ...products[idx], name, imageUrl: imageUrl || undefined, price: Math.max(0, price), active };
      setProducts(products);
    }

    modal.remove();
    renderManagement();
    renderProducts();
  };
}

function deleteProduct(productId) {
  if (!confirm('Delete this product?')) return;

  const products = getProducts().filter(p => String(p.id) !== String(productId));
  setProducts(products);
  renderManagement();
  renderProducts();
}

// Reports
function getOrders() {
  try {
    const data = localStorage.getItem(STORAGE_KEYS.orders);
    return data ? JSON.parse(data) : [];
  } catch {
    return [];
  }
}

function getMonthlySummary(orders, productsById) {
  const months = {};
  for (const order of orders) {
    const d = new Date(order.timestamp);
    const ym = `${d.getFullYear()}-${String(d.getMonth() + 1).padStart(2, '0')}`;
    if (!months[ym]) {
      months[ym] = { totalItems: 0, totalRevenue: 0, perProduct: {} };
    }
    for (const item of order.items) {
      months[ym].totalItems += item.quantity;
      months[ym].totalRevenue += (item.unitPrice || 0) * item.quantity;
      const pp = months[ym].perProduct[item.productId] || { qty: 0, revenue: 0 };
      pp.qty += item.quantity;
      pp.revenue += (item.unitPrice || 0) * item.quantity;
      months[ym].perProduct[item.productId] = pp;
    }
  }
  return months;
}

function renderReports() {
  const summaryEl = document.getElementById('reports-summary');
  const breakdownEl = document.getElementById('reports-breakdown');
  if (!summaryEl || !breakdownEl) return;

  const orders = getOrders();
  const products = getProducts();
  const productsById = Object.fromEntries(products.map(p => [String(p.id), p]));
  const months = getMonthlySummary(orders, productsById);
  const sortedMonths = Object.keys(months).sort().reverse();

  if (sortedMonths.length === 0) {
    summaryEl.innerHTML = '<p class="empty-state">No orders yet. Complete a purchase to see reports.</p>';
    breakdownEl.classList.add('hidden');
    return;
  }

  summaryEl.innerHTML = `
    <table class="reports-table">
      <thead>
        <tr>
          <th>Month</th>
          <th>Total Items Sold</th>
          <th>Total Revenue</th>
        </tr>
      </thead>
      <tbody>
        ${sortedMonths.map(ym => `
          <tr>
            <td>${ym}</td>
            <td class="breakdown-trigger" data-month="${ym}">${months[ym].totalItems}</td>
            <td>Rs. ${months[ym].totalRevenue}</td>
          </tr>
        `).join('')}
      </tbody>
    </table>
  `;

  breakdownEl.classList.add('hidden');

  summaryEl.querySelectorAll('.breakdown-trigger').forEach(cell => {
    cell.addEventListener('click', () => showBreakdown(cell.dataset.month, months, productsById, breakdownEl));
  });
}

function showBreakdown(month, months, productsById, breakdownEl) {
  const data = months[month];
  if (!data) return;

  const rows = Object.entries(data.perProduct).map(([productId, { qty, revenue }]) => {
    const product = productsById[productId];
    const name = product ? product.name : 'Unknown';
    return { name, qty, revenue };
  });

  breakdownEl.innerHTML = `
    <h2>Breakdown for ${month}</h2>
    <table class="reports-table">
      <thead>
        <tr>
          <th>Product</th>
          <th>Quantity Sold</th>
          <th>Revenue</th>
        </tr>
      </thead>
      <tbody>
        ${rows.map(r => `
          <tr>
            <td>${escapeHtml(r.name)}</td>
            <td>${r.qty}</td>
            <td>Rs. ${r.revenue}</td>
          </tr>
        `).join('')}
      </tbody>
    </table>
    <button class="btn btn-secondary close-breakdown" style="margin-top: 1rem;">Close</button>
  `;

  breakdownEl.classList.remove('hidden');
  breakdownEl.querySelector('.close-breakdown')?.addEventListener('click', () => {
    breakdownEl.classList.add('hidden');
  });
}

function updateCartBadge() {
  const cart = JSON.parse(localStorage.getItem(STORAGE_KEYS.cart) || '[]');
  const total = cart.reduce((sum, item) => sum + item.quantity, 0);
  const badge = document.getElementById('cart-count');
  if (badge) badge.textContent = total;
}

// Init
document.addEventListener('DOMContentLoaded', () => {
  initNavigation();
  initAuthForms();
  getUsers(); // ensure default admin exists
  applyAuthRestrictions();
  updateCartBadge();
  renderProducts();
});

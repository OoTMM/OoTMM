const dbPromise = openDatabase();

function openDatabase(): Promise<IDBDatabase> {
  return new Promise((resolve, reject) => {
    const request = indexedDB.open('ootmm', 1);
    request.onupgradeneeded = () => {
      const db = request.result;
      const os = db.createObjectStore('data', { keyPath: 'key' });
      os.createIndex('key', 'key', { unique: true });
    };
    request.onsuccess = () => {
      resolve(request.result);
    };
    request.onerror = () => {
      reject(request.error);
    };
  });
}

export async function saveFile(name: string, file: File | null): Promise<void> {
  const db = await dbPromise;
  return new Promise((resolve, reject) => {
    const store = db.transaction('data', 'readwrite').objectStore('data');
    if (file) {
      store.put({ key: name, data: file });
    } else {
      store.delete(name);
    }

    store.transaction.oncomplete = () => {
      resolve();
    };
    store.transaction.onerror = (e) => {
      reject(e);
    };
  });
}

export async function loadFile(name: string): Promise<File | null> {
  const db = await dbPromise;
  return new Promise((resolve, reject) => {
    const store = db.transaction('data').objectStore('data');
    const request = store.get(name);
    request.onsuccess = () => {
      resolve(request.result?.data ?? null);
    };
    request.onerror = (e) => {
      reject(e);
    };
  });
}

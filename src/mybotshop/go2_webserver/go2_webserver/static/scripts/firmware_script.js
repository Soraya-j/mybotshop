document.addEventListener('DOMContentLoaded', () => {
    const firmwareUploadForm = document.getElementById('firmware-upload-form');
    const firmwareFile = document.getElementById('firmware-file');
    const firmwareStatus = document.getElementById('firmware-status');

    const dropZone = document.getElementById('drop-zone');
    const fileInfo = document.getElementById('file-info');

    // --- Drag & Drop Functionality ---
    if (dropZone && firmwareFile && fileInfo) {
        // Prevent default drag behaviors
        ['dragenter', 'dragover', 'dragleave', 'drop'].forEach(eventName => {
            dropZone.addEventListener(eventName, preventDefaults, false);
        });

        // Highlight drop zone on drag over
        ['dragenter', 'dragover'].forEach(eventName => {
            dropZone.addEventListener(eventName, highlight, false);
        });

        ['dragleave', 'drop'].forEach(eventName => {
            dropZone.addEventListener(eventName, unhighlight, false);
        });

        // Handle dropped files
        dropZone.addEventListener('drop', handleDrop, false);

        // Handle manual file selection
        firmwareFile.addEventListener('change', function () {
            updateFileInfo(this.files[0]);
        });

        // Click drop zone to browse
        dropZone.addEventListener('click', function () {
            firmwareFile.click();
        });

        function preventDefaults(e) {
            e.preventDefault();
            e.stopPropagation();
        }

        function highlight(e) {
            dropZone.classList.add('drag-over');
        }

        function unhighlight(e) {
            dropZone.classList.remove('drag-over');
        }

        function handleDrop(e) {
            const dt = e.dataTransfer;
            const files = dt.files;

            if (files.length > 0) {
                firmwareFile.files = files; // Assign to input
                updateFileInfo(files[0]);
            }
        }

        function updateFileInfo(file) {
            if (file) {
                const validTypes = ['.zip', '.tar.gz', '.gz'];
                const fileName = file.name.toLowerCase();
                const isValid = validTypes.some(type => fileName.endsWith(type));

                if (isValid) {
                    fileInfo.textContent = `Selected: ${file.name} (${(file.size / 1024 / 1024).toFixed(2)} MB)`;
                    fileInfo.classList.add('has-file');
                    dropZone.style.borderColor = 'var(--project-color)';
                } else {
                    fileInfo.textContent = `Invalid file type. Please select .zip or .tar.gz file.`;
                    fileInfo.classList.remove('has-file');
                    firmwareFile.value = '';
                }
            } else {
                fileInfo.textContent = 'No file selected';
                fileInfo.classList.remove('has-file');
                dropZone.style.borderColor = 'var(--border-color)';
            }
        }
    }

    if (firmwareUploadForm) {
        firmwareUploadForm.addEventListener('submit', async (event) => {
            event.preventDefault();
            firmwareStatus.textContent = 'Uploading...';

            const file = firmwareFile.files[0];
            if (!file) {
                firmwareStatus.textContent = 'Please select a file.';
                return;
            }

            const formData = new FormData();
            formData.append('firmware_file', file);

            try {
                const response = await fetch('/upload_firmware', {
                    method: 'POST',
                    body: formData,
                });

                const result = await response.json();
                if (response.ok) {
                    firmwareStatus.textContent = `Success: ${result.message}`;
                } else {
                    firmwareStatus.textContent = `Error: ${result.message || 'Upload failed'}`;
                }
            } catch (error) {
                console.error('Upload error:', error);
                firmwareStatus.textContent = `Network error: ${error.message}`;
            }
        });
    }
});
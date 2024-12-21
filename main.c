#include <archive.h>
#include <archive_entry.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>

int add_to_archive(struct archive *archive, const char *path, const char *base_path) {
    struct stat st;
    struct archive_entry *entry;
    FILE *file;
    char buffer[1024];
    size_t bytes_read;

    // Get file or directory information
    if (stat(path, &st) != 0) {
        fprintf(stderr, "Error: Unable to stat %s\n", path);
        return 1;
    }

    // Create a new archive entry
    entry = archive_entry_new();
    archive_entry_set_pathname(entry, path + strlen(base_path) + 1); // Relative path in the archive
    archive_entry_set_size(entry, st.st_size);  // Set size for regular files
    archive_entry_set_filetype(entry, S_ISDIR(st.st_mode) ? AE_IFDIR : AE_IFREG);
    archive_entry_set_perm(entry, st.st_mode);

    // Write entry header to archive
    if (archive_write_header(archive, entry) != ARCHIVE_OK) {
        fprintf(stderr, "Error writing header: %s\n", archive_error_string(archive));
        archive_entry_free(entry);
        return 1;
    }

    // Write file content to archive (only for regular files)
    if (S_ISREG(st.st_mode)) {
        file = fopen(path, "rb");
        if (!file) {
            fprintf(stderr, "Error: Unable to open %s\n", path);
            archive_entry_free(entry);
            return 1;
        }

        while ((bytes_read = fread(buffer, 1, sizeof(buffer), file)) > 0) {
            if (archive_write_data(archive, buffer, bytes_read) < 0) {
                fprintf(stderr, "Error writing file data: %s\n", archive_error_string(archive));
                fclose(file);
                archive_entry_free(entry);
                return 1;
            }
        }
        fclose(file);
    }

    archive_entry_free(entry);

    // Recurse into directories
    if (S_ISDIR(st.st_mode)) {
        struct dirent *entry;
        DIR *dir = opendir(path);
        if (!dir) {
            fprintf(stderr, "Error: Unable to open directory %s\n", path);
            return 1;
        }

        while ((entry = readdir(dir)) != NULL) {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
                continue;  // Skip "." and ".."
            }

            char child_path[1024];
            snprintf(child_path, sizeof(child_path), "%s/%s", path, entry->d_name);
            if (add_to_archive(archive, child_path, base_path) != 0) {
                closedir(dir);
                return 1;
            }
        }

        closedir(dir);
    }

    return 0;
}

int compress_directory(const char *archive_name, const char *dir_path) {
    struct archive *archive;

    // Create a new archive in ZIP format with compression
    archive = archive_write_new();
    archive_write_set_format_zip(archive);
    archive_write_set_options(archive, "compression=deflate");  // Enable compression

    if (archive_write_open_filename(archive, archive_name) != ARCHIVE_OK) {
        fprintf(stderr, "Error creating archive: %s\n", archive_error_string(archive));
        archive_write_free(archive);
        return 1;
    }

    // Add the directory and its contents to the archive
    if (add_to_archive(archive, dir_path, dir_path) != 0) {
        fprintf(stderr, "Error adding directory to archive\n");
        archive_write_close(archive);
        archive_write_free(archive);
        return 1;
    }

    // Finalize and free resources
    archive_write_close(archive);
    archive_write_free(archive);

    printf("Successfully created archive: %s\n", archive_name);
    return 0;
}


int main() {
    const char *dir_path = "../example_directory";
    const char *archive_name = "../output.tar.xz";

    // Compress the directory
    return compress_directory(archive_name, dir_path);
}

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    
    <link href="https://fonts.googleapis.com/css2?family=Fira+Sans:ital,wght@0,100;0,200;0,300;0,400;0,500;0,600;0,700;0,800;0,900;1,100;1,200;1,300;1,400;1,500;1,600;1,700;1,800;1,900&display=swap" rel="stylesheet">
    
    <title>Document</title>

    <style>
        *{
            margin: 0;
            padding: 0;

            box-sizing: border-box;

            font-family: 'Fira Sans', sans-serif;

            scroll-behavior: smooth !important;
            transition: 300ms;
        }
        body{
            height: 100vh;

            padding: 20px;

            display: flex;
            justify-content: center;
            align-items: center;
        }
        .application{
            padding: 30px;

            display: flex;
            flex-direction: column;
            gap: 35px;

            background-color: #eee;
            border-radius: 40px;
        }
        .application, form{
            display: flex;
            flex-direction: column;
            justify-content: center;
            align-items: center;
        }
        
        .dropzone{
            padding: 60px 20px;

            display: flex;
            flex-direction: column;
            justify-content: center;
            align-items: center;
            gap: 10px;

            background-color: #fff;
            border-radius: 10px;

            border: 2px dashed #dadada;

            box-shadow: 0 0 0 10px #fff;
        }

        .dropzone  span{
            color: #969696;
        }

        .sub{
            margin: 35px 0 0 0;
            padding: 10px 20px;
            border: none;

            background-color: #00ffdb;
            border-radius: 20px;
            box-shadow: 0 0 0 10px #b8fbf1;

            font-size: 1em;
            cursor: pointer;
        }
    </style>
</head>
<body>
    <main class="application">
        <h1>LaTex Converter</h1>
        <form action="" method="post">
            <label for="dropzone" class="dropzone">
                <span>Glissez votre fichier</span>
                <span>ou</span>
                <span>Cliquez sur la zone pour parcourir vos fichiers</span>
                <input type="file" name="file" id="file" class="dropzone-input">
            </label>
            <input type="submit" name="submit" value="Convertir" class="sub">
        </form>

        <?php
            if(isset($_POST["submit"])){
                if($_FILES["file"]["error"] > 0){
                    echo "Error: " . $_FILES["file"]["error"] . "<br>";
                }else{
                    $target_dir = "uploads/";
                    $target_file = $target_dir . basename($_FILES["file"]["name"]);
                    $file_type = pathinfo($target_file,PATHINFO_EXTENSION);
    
                    // Vérifier si le fichier est un fichier texte valide ou un fichier DOC/DOCX
                    if($file_type != "txt" && $file_type != "doc" && $file_type != "docx"){
                        echo "Error: Seuls les fichiers TXT, DOC et DOCX sont autorisés.";
                    } else {
                        // télécharger le fichier original
                        if (move_uploaded_file($_FILES["file"]["tmp_name"], $target_file)) {
                            echo "Le fichier ". basename( $_FILES["file"]["name"]). " a été téléchargé avec succès.";
    
                            // Convertir le fichier en C
                            $c_target_file = $target_dir . str_replace($file_type, 'c', basename($_FILES["file"]["name"])); // générer un nom de fichier cible pour le fichier converti
                            $command = "/usr/bin/convert_program -i $target_file -o $c_target_file"; // command pour convertir le fichier
                            exec($command);
    
                            // Vérifier si le fichier converti existe et le télécharger
                            if (file_exists($c_target_file)) {
                                header('Content-Type: application/octet-stream');
                                header('Content-Disposition: attachment; filename="' . basename($c_target_file) . '"');
                                header('Content-Length: ' . filesize($c_target_file));
                                readfile($c_target_file);
                            } else {
                                echo "Erreur: le fichier converti n'existe pas.";
                            }
                        } else {
                            echo "Erreur lors du téléchargement du fichier.";
                        }
                    }
                }
            }
        ?>
    </main>

</body>
</html>
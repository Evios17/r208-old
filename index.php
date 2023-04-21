<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    
    <link href="https://fonts.googleapis.com/css2?family=Fira+Sans:ital,wght@0,100;0,200;0,300;0,400;0,500;0,600;0,700;0,800;0,900;1,100;1,200;1,300;1,400;1,500;1,600;1,700;1,800;1,900&display=swap" rel="stylesheet">
    
    <title>Document</title>

    <script src="https://ajax.googleapis.com/ajax/libs/jquery/2.2.4/jquery.min.js"></script>

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

            background-color: #eeeeee;
        }







        .loader-layout{
            width: 100%;
            height: 100vh;

            display: flex;
            justify-content: center;
            align-items: center;

            position: fixed;
            top: 0;
            left: 0;

            background-color: #eeeeee27;
            backdrop-filter: blur(10px);

            font-size: 18px;

            z-index: 1000;
        }

        .loader-box{
            padding: 30px 40px;

            background-color: #ffffff;

            border-radius: 20px;

            font-size: 1.1em;
        }

        .loader {
            width: 45px;
            height: 45px;

            margin: 0 auto 30px auto;

            display: flex;
            justify-content: center;
            align-items: center;

            position: relative;
            
            background-color: #ccccff;
            border-radius: 50%;

            animation: loader 1s infinite alternate;
        }

        .loader:before{
            content: "";

            width: 25px;
            height: 25px;

            display: block;

            position: absolute;

            background-color: #afafd8;
            border-radius: 50%;

            animation: loader 1s infinite alternate;
            animation-delay: 0.5s;
        }

        @keyframes loader {
            from {
                transform: scale(0.8);
            } to {
                transform: scale(1.3);
            }
        }








        .application{
            padding: 30px;

            display: flex;
            flex-direction: column;
            gap: 35px;

            background-color: #ffffff;
            box-shadow: 0 0 20px 0 #dadada;
            border-radius: 40px;
        }
        
        .dropzone{
            aspect-ratio: 16 / 10;
            width: 100%;

            margin: auto;
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

        .highlight{
            background-color: #0000ff;
        }

        .indiquator{
            margin: 20px 0 0 0;
            
        }

        .sub{
            margin: 35px auto 0 auto;
            padding: 10px 20px;

            display: block;

            border: none;

            background-color: #ccccff;
            border-radius: 20px;
            box-shadow: 0 0 0 5px #e4e4ff;

            font-size: 1em;
            cursor: pointer;
        }

        .sub:hover{
            box-shadow: 0 0 0 10px #e4e4ff;
        }

        .title{
            margin: 0 0 6px 0;
            font-size: 1.6em;
        }

        .subtitle{
            font-size: 1em;
        }

        .description{
            font-size: 1em;
            color: #a3a3a3;
        }








        .drag-image{
            width: 100%;

            padding: 20px 20px 40px 20px;

            display: flex;
            align-items: center;
            justify-content: center;
            flex-direction: column;

            border: 2px dashed #dadada;
            border-radius: 20px;

            font-weight:400;
            
        }
        .drag-image.active{
            border: 2px solid #ff0000;
        }
        .drag-image .icon{
            font-size: 30px;
            color: #ff0000;
        }
        .drag-image .separator{
            margin: 15px 0 20px 0;

            font-weight: bold;
        }
        .drag-image button{
            padding: 10px 15px;
            
            display: block;
            
            background-color: #ccccff;
            box-shadow: 0 0 0 5px #e4e4ff;
            border: none;
            border-radius: 20px;

            font-size: 1em;

            cursor: pointer;
        }

        .drag-image button:hover{
            box-shadow: 0 0 0 10px #e4e4ff;
        }

        .drag-image img{
            height: 100%;
            width: 100%;
            object-fit: cover;
            border-radius: 5px;
        }
    </style>
</head>
<body>
    <div class="loader-layout">
        <div class="loader-box">
            <div class="loader"></div>
            <span>Chargement</span>
        </div>
    </div>

    <main class="application">
        <form action="" method="post">
            <div class="drag-image">
                <img src="sources/media/file.png" alt="" width="80px" style="display: none;">
                <span class="indiquator">Drag and drop file here.</span>
                <div class="icon"><i class="fas fa-cloud-upload-alt"></i></div>
                <span class="separator">OR</span>
                <button>Browse File</button>
                <input type="file" name="file" id="file" hidden>
            </div>
        </form>
        <div>
            <h1 class="title">BlackLaTex</h1>
            <span class="subtitle">Converter PGN > LaTex.</span>
        </div>
        <span class="description">Once uploaded, your file will be converted automatically.</span>

        <?php
            if(isset($_POST["submit"])){
                if(isset($_FILES['file'])) {
                    $targetDir = "input/";
                    $fileName = basename($_FILES['file']['name']);
                    $targetFilePath = $targetDir . $fileName;
                    $fileType = pathinfo($targetFilePath, PATHINFO_EXTENSION);
                
                    if(move_uploaded_file($_FILES["file"]["tmp_name"], $targetFilePath)) {
                        echo "Le fichier " . $fileName . " a été téléchargé.";
                    } else {
                        echo "Une erreur s'est produite lors du téléchargement du fichier.";
                    }
                }
            }
        ?>
    </main>

    <script>

        $(window).load(function() {
            $(".loader-layout").fadeOut("slow");;
        });




		const dropArea = document.querySelector(".drag-image"),
        dragText = dropArea.querySelector("h6"),
        button = dropArea.querySelector("button"),
        input = dropArea.querySelector("input");
        let file; 

        button.onclick = ()=>{
        input.click(); 
        }

        input.addEventListener("change", function(){
        
        file = this.files[0];
        dropArea.classList.add("active");
        viewfile();
        });

        dropArea.addEventListener("dragover", (event)=>{
        event.preventDefault();
        dropArea.classList.add("active");
        dragText.textContent = "Release to Upload File";
        });


        dropArea.addEventListener("dragleave", ()=>{
        dropArea.classList.remove("active");
        dragText.textContent = "Drag & Drop to Upload File";
        }); 

        dropArea.addEventListener("drop", (event)=>{
        event.preventDefault(); 
        
        file = event.dataTransfer.files[0];
        viewfile(); 
        });

        function viewfile(){
        let fileType = file.type; 
        let validExtensions = ["image/jpeg", "image/jpg", "image/png"];
        if(validExtensions.includes(fileType)){ 
            let fileReader = new FileReader(); 
            fileReader.onload = ()=>{
            let fileURL = fileReader.result; 
            let imgTag = `<img src="${fileURL}" alt="image">`;
            dropArea.innerHTML = imgTag; 
            }
            fileReader.readAsDataURL(file);
        }else{
            alert("This is not an Image File!");
            dropArea.classList.remove("active");
            dragText.textContent = "Drag & Drop to Upload File";
        }
        }
	</script>

</body>
</html>
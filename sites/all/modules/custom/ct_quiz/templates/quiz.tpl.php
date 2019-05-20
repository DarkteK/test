<div id="main_quiz" class="container">
  <div class="row">
    <div class="page_title col-lg text-center">
        <h2>{{quiz_title}}</h2>
    </div>
  </div>
  <div id="data_render" v-if="quiz_list" >
    <?php foreach ($quiz_list_nodes as $quiz) { ?>
    <div class="row">
      <div class="col-lg quiz text-center" data-quiz="<?php echo $quiz->nid; ?>" @click="loadQuiz">
        <div>Click to open Quiz <?php echo $quiz->nid; ?>º</div>
      </div>
    </div>
    <?php } ?>
  </div>
  <div v-else>
    <div id="data_quiz" v-html="quiz_output"></div>
    <div class="row">
      <button type="button" v-if="questions[questions_counter]" class="btn btn-primary" @click="checkAnswer">Next</button>
    </div>

    <div class="row">
      <button type="button" class="btn btn-primary" @click="quiz_list = !quiz_list">Return to list</button>
    </div>
  </div>
</div>

<script>
  const link = Drupal.settings.basePath + 'api/quiz?quiz=';

  new Vue({
      el: '#main_quiz',
      data: {
          questions: [],
          quiz: [],
          quiz_title: "Quizzes",
          quiz_output: "",
          quiz_list: true,
          questions_counter: 0,
          answers_counter: 0,
      },
      methods:{
        loadQuiz(e) {
          let quizId = e.currentTarget.getAttribute('data-quiz');

          axios
            .get(link + quizId)
            .then(response => {
              if (response.status === 200 && response.data.code === 0){
                this.quiz = response.data.data;

                this.quiz_list = false;
                this.prepareData();
              }else{
                this.quiz = null;
              }
            })
            .catch(error => {
              this.quiz = null;
            })
        },
        prepareData() {
          this.questions_counter = 0;

          this.quiz.qa.map(quizData => {
            let questionsAnswers = new Object();
            let answers = new Array();
            
            questionsAnswers.title = 
              '<div class="row">' +
              '<div class="quiz_question col-lg text-center">' +
                'Question: ' + quizData.field_quiz_fc_question +
              '</div>' +
            '</div>';

            questionsAnswers.message = 
              '<div class="row">' +
              '<div class="quiz_message col-lg text-center">' +
              '</div>' +
            '</div>';

            quizData.field_quiz_fc_answers.map(quizAnswers => {
              
             answers[this.answers_counter]= 
              '<div class="row">' +
                '<div class="col-lg text-center">' +
                  '<label for="' + quizAnswers.answer + '"> ' +
                    "<input type='radio' name='answers_" + this.questions_counter + "' value='" + quizAnswers.correct + "'>" + quizAnswers.answer +
                    '</label>' +
                '</div>' +
              '</div>';
              
              this.answers_counter++;
            
            });
            questionsAnswers.answers = answers;
            this.answers_counter = 0;

            this.questions[this.questions_counter] = questionsAnswers;
            this.questions_counter++;

          });

          this.renderData(true);

        },
        renderData(flag) {
          
          if (flag) {
            this.questions_counter = 0;
          }
          if (this.questions[this.questions_counter]) {

            this.quiz_output = this.questions[this.questions_counter].title + this.questions[this.questions_counter].answers.join(' ') + this.questions[this.questions_counter].message;
          }
          else {
            this.quiz_output = 
            '<div class="row">' +
              '<div class="quiz_message col-lg text-center">' +
                'Congratulations, you\'ve completed the test' +
              '</div>' +
            '</div>';
          }
        },
        checkAnswer() {
          var radioValue = jQuery("input[name='answers_" + this.questions_counter + "']:checked").val();

          if (radioValue === undefined) {
            jQuery("div.quiz_message").html("Please select an answer.")
          }
          else {
            if (radioValue == 1) {
              this.questions_counter++;
              this.renderData(false);
            }
            else {
              jQuery("div.quiz_message").html("Wrong answer, please try again.")
            }
          }
        }
      },
      watch:{
        quiz_list() {
          if (!this.quiz_list) {
            this.quiz_title = this.quiz.title;
            document.title = this.quiz.title;
          }
          else {
            this.quiz_title = "Quizzes";
            document.title = "Quizzes Landing Page";
          }
        }
      }
    });
</script>

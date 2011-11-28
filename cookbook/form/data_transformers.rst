データトランスフォーマの使用
=======================

ユーザがフォームで入力したデータを、プログラム内で使用しやすいように変換する必要があるときもあります。コントローラで手動で変換することも簡単にできますが、他の場所でもこの特別なフォームを使用したいとき
など汎用性の必要なときも考えてみましょう。

one-to-one のリレーションを持つ Task と Issue があったとします。例えば Task はオプションとして関連する Issue を保持します。全ての可能性のある issues にリストボックスを追加してしまうと、最終的にリストボックス内を探すのに困難なくらい長くなってしまいます。リストボックスの代わりに、ユーザが issue のナンバーを単純に入力したテキストボックスを追加した方が良くみえます。コントローラ内でこの Issue のナンバーを実際の Task に変換することもでき、その際に Task が見つからなければエラーを返すこともできますが、これはあまり綺麗な実装ではありません。

実装するアクション内でこの issue を実際に調べ Issue オブジェクトに変換する方がスマートでしょう。この役割を担ってくれるのがデータトランスフォーマです。

まず、データトランスフォーマに伴うカスタムフォームタイプを作成してください。このデータトランスフォーマは、 Issue を issue セレクタタイプのナンバーで返します。フィールドの親を "text" フィールドに設定します。そうすると、最終的には、カスタムフォームタイプは、Issue のナンバーを入力することになるテキストフィールドになります。存在していないナンバーが入力された際には、フィールドはエラーを表示します。
::

    // src/Acme/TaskBundle/Form/IssueSelectorType.php
    namespace Acme\TaskBundle\Form\Type;
    
    use Symfony\Component\Form\AbstractType;
    use Symfony\Component\Form\FormBuilder;
    use Acme\TaskBundle\Form\DataTransformer\IssueToNumberTransformer;
    use Doctrine\Common\Persistence\ObjectManager;

    class IssueSelectorType extends AbstractType
    {
        private $om;
    
        public function __construct(ObjectManager $om)
        {
            $this->om = $om;
        }
    
        public function buildForm(FormBuilder $builder, array $options)
        {
            $transformer = new IssueToNumberTransformer($this->om);
            $builder->appendClientTransformer($transformer);
        }
    
        public function getDefaultOptions(array $options)
        {
            return array(
                'invalid_message'=>'The selected issue does not exist'
            );
        }
    
        public function getParent(array $options)
        {
            return 'text';
        }
    
        public function getName()
        {
            return 'issue_selector';
        }
    }

.. tip::

    新しくカスタムフォームタイプを作成しなくても、 ``appendClientTransformer`` をフィールドビルダーで呼ぶことでトランスフォーマを使用することができます。
::

        use Acme\TaskBundle\Form\DataTransformer\IssueToNumberTransformer;

        class TaskType extends AbstractType
        {
            public function buildForm(FormBuilder $builder, array $options)
            {
                // ...
            
                // this assumes that the entity manager was passed in as an option
                $entityManager = $options['em'];
                $transformer = new IssueToNumberTransformer($entityManager);

                // use a normal text field, but transform the text into an issue object
                $builder
                    ->add('issue', 'text')
                    ->appendClientTransformer($transformer)
                ;
            }
            
            // ...
        }

次に実際の変換を行うデータトランスフォーマを作成します。
::

    // src/Acme/TaskBundle/Form/DataTransformer/IssueToNumberTransformer.php
    namespace Acme\TaskBundle\Form\DataTransformer;
    
    use Symfony\Component\Form\Exception\TransformationFailedException;
    use Symfony\Component\Form\DataTransformerInterface;
    use Doctrine\Common\Persistence\ObjectManager;
    
    class IssueToNumberTransformer implements DataTransformerInterface
    {
        private $om;

        public function __construct(ObjectManager $om)
        {
            $this->om = $om;
        }

        // Issue オブジェクトを文字列に変換します
        public function transform($val)
        {
            if (null === $val) {
                return '';
            }

            return $val->getNumber();
        }

        // issue ナンバーを Issue オブジェクトに変換します
        public function reverseTransform($val)
        {
            if (!$val) {
                return null;
            }

            $issue = $this->om->getRepository('AcmeTaskBundle:Issue')->findOneBy(array('number' => $val));

            if (null === $issue) {
                throw new TransformationFailedException(sprintf('An issue with number %s does not exist!', $val));
            }

            return $issue;
        }
    }

最後に、データトランスフォーマを使ったカスタムフォームタイプを作成するようにしたので、サービスコンテナにそのタイプを登録し、エンティティマネージャが自動的に注入(inject)できるようにします。

.. configuration-block::

    .. code-block:: yaml

        services:
            acme_demo.type.issue_selector:
                class: Acme\TaskBundle\Form\IssueSelectorType
                arguments: ["@doctrine.orm.entity_manager"]
                tags:
                    - { name: form.type, alias: issue_selector }

    .. code-block:: xml
    
        <service id="acme_demo.type.issue_selector" class="Acme\TaskBundle\Form\IssueSelectorType">
            <argument type="service" id="doctrine.orm.entity_manager"/>
            <tag name="form.type" alias="issue_selector" />
        </service>

これで次のようにエイリアスでフォームにタイプを追加できるようになりました。
::

    // src/Acme/TaskBundle/Form/Type/TaskType.php
    
    namespace Acme\TaskBundle\Form\Type;
    
    use Symfony\Component\Form\AbstractType;
    use Symfony\Component\Form\FormBuilder;
    
    class TaskType extends AbstractType
    {
        public function buildForm(FormBuilder $builder, array $options)
        {
            $builder->add('task');
            $builder->add('dueDate', null, array('widget' => 'single_text'));
            $builder->add('issue', 'issue_selector');
        }
    
        public function getName()
        {
            return 'task';
        }
    }

これで、アプリケーション内のどこでも、ナンバーで issue を選択するセレクタタイプを使用することが簡単にできるようになりました。

新しく未知のナンバーが入力された際に issue を新しく作成したければ、 TransformationFailedException 例外を投げるのではなく Issue インスタンスを初期化することもできます。さらには、 task が issue へのカスケーディングのオプションを指定していなければエンティティマネージャに保存することもできます。

.. 2011/11/28 ganchiku 9bd55cb80fd21e47a3a2f97c7be9d6d97926ecff
